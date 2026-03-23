/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//----------------------------------------------------------------------------
// P5-01 — OpenAL Miles Sound System Shim
//
// Implements every AIL_ function declared (not defined) in MilesSoundStub.h
// when ZH_OPENAL_AUDIO is defined.  MilesAudioManager.cpp is unchanged; it
// continues to call the same AIL_ function names, which now dispatch to
// real OpenAL calls.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
//----------------------------------------------------------------------------

#define STUB_IMPL
#define ZH_OPENAL_AUDIO
#include "MilesSoundStub.h"   // extern declarations only in this mode

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX
#include <windows.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdint>
#include <algorithm>

//----------------------------------------------------------------------------
// Global OpenAL device + context
//----------------------------------------------------------------------------

static ALCdevice*  g_device  = NULL;
static ALCcontext* g_context = NULL;

//----------------------------------------------------------------------------
// Registered file I/O callbacks (set by MilesAudioManager via
// AIL_set_file_callbacks so we can read from TheFileSystem, not raw disk)
//----------------------------------------------------------------------------

static AILFOPEN  g_fileOpen  = NULL;
static AILFCLOSE g_fileClose = NULL;
static AILFSEEK  g_fileSeek  = NULL;
static AILFREAD  g_fileRead  = NULL;

//----------------------------------------------------------------------------
// WAV format helpers
//----------------------------------------------------------------------------

#pragma pack(push,1)
struct WavRiff { char riff[4]; DWORD size; char wave[4]; };
struct WavFmt  { char id[4]; DWORD size; WORD format; WORD channels;
                 DWORD sampleRate; DWORD byteRate; WORD blockAlign;
                 WORD bitsPerSample; };
struct WavData { char id[4]; DWORD size; };
#pragma pack(pop)

static bool parseWav(const void* buf, DWORD bufLen,
                     WORD* outFmt, WORD* outCh, DWORD* outRate,
                     WORD* outBits, WORD* outBlock,
                     const void** outData, DWORD* outDataLen)
{
    if (!buf || bufLen < sizeof(WavRiff)) return false;
    const BYTE* p   = (const BYTE*)buf;
    const BYTE* end = p + bufLen;
    if (memcmp(p, "RIFF", 4) != 0 || memcmp(p+8, "WAVE", 4) != 0) return false;
    p += 12;
    *outFmt = 0; *outCh = 0; *outRate = 0; *outBits = 0; *outBlock = 0;
    *outData = NULL; *outDataLen = 0;
    while (p + 8 <= end) {
        const char* id   = (const char*)p;
        DWORD       csz  = *(DWORD*)(p+4);
        p += 8;
        if (memcmp(id, "fmt ", 4) == 0 && csz >= 16) {
            *outFmt   = *(WORD*)(p);
            *outCh    = *(WORD*)(p+2);
            *outRate  = *(DWORD*)(p+4);
            *outBits  = *(WORD*)(p+14);
            *outBlock = *(WORD*)(p+12);
        } else if (memcmp(id, "data", 4) == 0) {
            *outData    = p;
            *outDataLen = csz;
        }
        // align to word boundary
        p += (csz + 1) & ~1u;
        if (*outData && *outFmt) break; // have what we need
    }
    return (*outFmt != 0 && *outData != NULL);
}

// Write a 44-byte PCM WAV header into dst (must be at least 44 bytes)
static void writePcmHeader(BYTE* dst, DWORD dataBytes,
                            WORD channels, DWORD rate, WORD bits)
{
    DWORD byteRate   = rate * channels * (bits / 8);
    WORD  blockAlign = (WORD)(channels * (bits / 8));
    memcpy(dst,    "RIFF", 4);
    *(DWORD*)(dst+4)  = 36 + dataBytes;
    memcpy(dst+8,  "WAVE", 4);
    memcpy(dst+12, "fmt ", 4);
    *(DWORD*)(dst+16) = 16;
    *(WORD*) (dst+20) = 1;            // PCM
    *(WORD*) (dst+22) = channels;
    *(DWORD*)(dst+24) = rate;
    *(DWORD*)(dst+28) = byteRate;
    *(WORD*) (dst+32) = blockAlign;
    *(WORD*) (dst+34) = bits;
    memcpy(dst+36, "data", 4);
    *(DWORD*)(dst+40) = dataBytes;
}

//----------------------------------------------------------------------------
// IMA ADPCM decoder
//----------------------------------------------------------------------------

static const int s_stepTable[89] = {
    7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,34,37,41,45,50,55,60,66,73,
    80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,337,371,408,449,
    494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,1707,1878,2066,
    2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,5894,6484,7132,7845,
    8630,9493,10442,11487,12635,13899,15289,16818,18500,20350,22385,24623,
    27086,29794,32767
};
static const int s_indexTable[16] = {
    -1,-1,-1,-1,2,4,6,8,-1,-1,-1,-1,2,4,6,8
};

static inline int clamp16(int v) {
    if (v >  32767) return  32767;
    if (v < -32768) return -32768;
    return v;
}
static inline int clampIdx(int i) {
    if (i < 0)  return 0;
    if (i > 88) return 88;
    return i;
}

// Decode one block of IMA ADPCM (mono or stereo).
// blockData points to the block. blockSize = info->block_size.
// outPcm receives S16 samples; outSamples is count (per channel).
static void decodeAdpcmBlock(const BYTE* blockData, DWORD blockSize,
                              WORD channels,
                              S16* outPcm, DWORD* outSamples)
{
    *outSamples = 0;
    if (!blockData || blockSize < 4u) return;

    // Each channel has a 4-byte preamble: S16 sample + U8 index + U8 reserved
    if (blockSize < (DWORD)(4 * channels)) return;

    // Channel state
    int pred[2]  = { 0, 0 };
    int index[2] = { 0, 0 };

    for (WORD ch = 0; ch < channels && ch < 2; ++ch) {
        pred[ch]  = (S16)(*(S16*)(blockData + ch*4));
        index[ch] = clampIdx(blockData[ch*4 + 2]);
    }

    DWORD headerBytes = 4u * channels;
    const BYTE* data  = blockData + headerBytes;
    DWORD dataBytes   = blockSize - headerBytes;

    // Number of nibbles = dataBytes * 2
    // For stereo: nibbles alternate LRLRLR... per byte pair
    DWORD nibbleCount = dataBytes * 2u;
    S16*  out         = outPcm;

    if (channels == 1) {
        // emit the preamble sample
        *out++ = (S16)pred[0];
        ++(*outSamples);
        for (DWORD n = 0; n < nibbleCount; ++n) {
            int nibble = (data[n/2] >> ((n&1) ? 4 : 0)) & 0x0F;
            int step   = s_stepTable[index[0]];
            int diff   = step >> 3;
            if (nibble & 4) diff += step;
            if (nibble & 2) diff += step >> 1;
            if (nibble & 1) diff += step >> 2;
            if (nibble & 8) pred[0] -= diff; else pred[0] += diff;
            pred[0]  = clamp16(pred[0]);
            index[0] = clampIdx(index[0] + s_indexTable[nibble]);
            *out++ = (S16)pred[0];
            ++(*outSamples);
        }
    } else {
        // Stereo: blocks come in pairs of 4 bytes (8 nibbles per channel per group)
        *out++ = (S16)pred[0];
        *out++ = (S16)pred[1];
        *outSamples += 2;
        // data bytes come in 8-byte groups: 4 bytes L, 4 bytes R
        DWORD groups = dataBytes / 8u;
        for (DWORD g = 0; g < groups; ++g) {
            const BYTE* grpL = data + g*8 + 0;
            const BYTE* grpR = data + g*8 + 4;
            for (int n = 0; n < 8; ++n) {
                for (WORD ch = 0; ch < 2; ++ch) {
                    const BYTE* grp = (ch == 0) ? grpL : grpR;
                    int nibble = (grp[n/2] >> ((n&1) ? 4 : 0)) & 0x0F;
                    int step   = s_stepTable[index[ch]];
                    int diff   = step >> 3;
                    if (nibble & 4) diff += step;
                    if (nibble & 2) diff += step >> 1;
                    if (nibble & 1) diff += step >> 2;
                    if (nibble & 8) pred[ch] -= diff; else pred[ch] += diff;
                    pred[ch]  = clamp16(pred[ch]);
                    index[ch] = clampIdx(index[ch] + s_indexTable[nibble]);
                    *out++ = (S16)pred[ch];
                    ++(*outSamples);
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
// OALSample — wraps a 2D (non-positional) OpenAL source
//----------------------------------------------------------------------------

struct OALSample {
    ALuint       src;
    ALuint       buf;          // single static buffer (not for streaming)
    void*        userData[8];
    AILSAMPLECB  eosCallback;
    F32          volume;
    F32          pan;
    bool         playing;

    OALSample() : src(0), buf(0), eosCallback(NULL),
                  volume(1.f), pan(0.f), playing(false)
    { memset(userData, 0, sizeof(userData)); }
};

//----------------------------------------------------------------------------
// OAL3DSample — wraps a 3D OpenAL source
//----------------------------------------------------------------------------

struct OAL3DSample {
    ALuint        src;
    ALuint        buf;
    void*         userData[8];
    AIL3DSAMPLECB eosCallback;
    F32           volume;
    bool          playing;

    OAL3DSample() : src(0), buf(0), eosCallback(NULL),
                    volume(1.f), playing(false)
    { memset(userData, 0, sizeof(userData)); }
};

//----------------------------------------------------------------------------
// OALListener — wraps the 3D listener
//----------------------------------------------------------------------------

struct OALListener {
    bool isListener; // used to distinguish from OAL3DSample in H3DPOBJECT casts
    OALListener() : isListener(true) {}
};

//----------------------------------------------------------------------------
// OALStream — wraps a streaming source with a background feeder thread
//----------------------------------------------------------------------------

#define OAL_STREAM_BUFFERS   2
#define OAL_STREAM_BUFSIZE   (32 * 1024)   // 32 KB per buffer

struct OALStream {
    ALuint       src;
    ALuint       bufs[OAL_STREAM_BUFFERS];
    char         filename[MAX_PATH];
    U32          fileHandle;
    WORD         channels;
    DWORD        sampleRate;
    WORD         bitsPerSample;
    AILSTREAMCB  callback;
    F32          volume;
    F32          pan;
    S32          loopCount;    // 0 = once, -1 = infinite, N = N times
    S32          loopsRemaining;
    bool         paused;
    bool         stopThread;
    HANDLE       thread;
    CRITICAL_SECTION cs;

    OALStream() : src(0), fileHandle(0), channels(1), sampleRate(22050),
                  bitsPerSample(16), callback(NULL),
                  volume(1.f), pan(0.f), loopCount(0), loopsRemaining(0),
                  paused(false), stopThread(false), thread(NULL)
    {
        memset(filename, 0, sizeof(filename));
        memset(bufs, 0, sizeof(bufs));
        InitializeCriticalSection(&cs);
    }
    ~OALStream()
    {
        DeleteCriticalSection(&cs);
    }
};

//----------------------------------------------------------------------------
// Stream feeder thread — runs until stopThread is set
//----------------------------------------------------------------------------

static DWORD WINAPI streamThread(LPVOID param)
{
    OALStream* s = (OALStream*)param;
    BYTE* pcmBuf = (BYTE*)malloc(OAL_STREAM_BUFSIZE);
    if (!pcmBuf) return 1;

    while (!s->stopThread) {
        EnterCriticalSection(&s->cs);
        if (s->paused || !g_fileRead) {
            LeaveCriticalSection(&s->cs);
            Sleep(10);
            continue;
        }

        // How many buffers are processed and ready for re-queueing?
        ALint processed = 0;
        alGetSourcei(s->src, AL_BUFFERS_PROCESSED, &processed);

        while (processed-- > 0 && !s->stopThread) {
            ALuint buf;
            alSourceUnqueueBuffers(s->src, 1, &buf);

            DWORD bytesRead = g_fileRead(s->fileHandle, pcmBuf, OAL_STREAM_BUFSIZE);
            if (bytesRead == 0) {
                // EOF — handle loop or end
                if (s->loopsRemaining != 0) {
                    if (s->loopsRemaining > 0) --s->loopsRemaining;
                    g_fileSeek(s->fileHandle, 44, 0); // seek past WAV header
                    bytesRead = g_fileRead(s->fileHandle, pcmBuf, OAL_STREAM_BUFSIZE);
                }
            }

            if (bytesRead > 0) {
                ALenum fmt = (s->channels == 2)
                    ? ((s->bitsPerSample == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8)
                    : ((s->bitsPerSample == 16) ? AL_FORMAT_MONO16   : AL_FORMAT_MONO8);
                alBufferData(buf, fmt, pcmBuf, (ALsizei)bytesRead, (ALsizei)s->sampleRate);
                alSourceQueueBuffers(s->src, 1, &buf);

                ALint state;
                alGetSourcei(s->src, AL_SOURCE_STATE, &state);
                if (state != AL_PLAYING && !s->paused)
                    alSourcePlay(s->src);
            } else {
                // Stream finished
                s->stopThread = true;
                if (s->callback)
                    s->callback((HSTREAM)s);
            }
        }

        LeaveCriticalSection(&s->cs);
        Sleep(5);
    }

    free(pcmBuf);
    return 0;
}

//----------------------------------------------------------------------------
// Helper: map an AILSOUNDINFO into an AL format enum
//----------------------------------------------------------------------------

static ALenum oalFormat(WORD channels, WORD bits)
{
    if (channels == 2)
        return (bits == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;
    return (bits == 16) ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8;
}

//----------------------------------------------------------------------------
// Helper: apply volume + pan to a 2D source
// Pan: -1.0 = full left, 0.0 = centre, +1.0 = full right
// We use the "constant-power" positioning trick: place the source in the XZ
// plane relative to the listener at unit distance.
//----------------------------------------------------------------------------

static void applyVolPan(ALuint src, F32 vol, F32 pan)
{
    alSourcef(src, AL_GAIN, vol < 0.f ? 0.f : vol);
    alSourcei(src, AL_SOURCE_RELATIVE, AL_TRUE);
    float px = pan;
    float pz = -sqrtf(std::max(0.f, 1.f - pan*pan));
    alSource3f(src, AL_POSITION, px, 0.f, pz);
}

//============================================================================
//  AIL_ implementations
//============================================================================

// --- Init / shutdown -------------------------------------------------------

char const* AIL_MSS_version(void)
{
    return "OpenAL Shim 1.0 (P5-01)";
}

S32 AIL_startup(void)
{
    if (g_device) return 1;
    g_device = alcOpenDevice(NULL);
    if (!g_device) return 0;
    g_context = alcCreateContext(g_device, NULL);
    if (!g_context) { alcCloseDevice(g_device); g_device = NULL; return 0; }
    alcMakeContextCurrent(g_context);
    // Listener defaults: at origin, facing -Z, up +Y
    alListener3f(AL_POSITION,    0.f, 0.f, 0.f);
    alListener3f(AL_VELOCITY,    0.f, 0.f, 0.f);
    float orient[6] = { 0.f, 0.f, -1.f,  0.f, 1.f, 0.f };
    alListenerfv(AL_ORIENTATION, orient);
    return 1;
}

void AIL_shutdown(void)
{
    if (g_context) { alcMakeContextCurrent(NULL); alcDestroyContext(g_context); g_context = NULL; }
    if (g_device)  { alcCloseDevice(g_device); g_device = NULL; }
}

S32  AIL_quick_startup(void)                              { return AIL_startup(); }
S32  AIL_quick_startup(S32, S32, U32, S32, S32)          { return AIL_startup(); }
void AIL_quick_handles(HDIGDRIVER* pdig, HMDIDRIVER* pmdi)
    { if(pdig) *pdig=(HDIGDRIVER)1; if(pmdi) *pmdi=NULL; }
void AIL_quick_handles(HDIGDRIVER* pdig, HMDIDRIVER* pmdi, HDIGDRIVER* pdig2)
    { if(pdig) *pdig=(HDIGDRIVER)1; if(pmdi) *pmdi=NULL; if(pdig2) *pdig2=(HDIGDRIVER)1; }
void AIL_set_redist_directory(char const*) {}
U32  AIL_get_timer_highest_delay(void) { return 0; }

// --- File I/O callbacks ----------------------------------------------------

void AIL_set_file_callbacks(AILFOPEN open_cb, AILFCLOSE close_cb,
                             AILFSEEK seek_cb,  AILFREAD  read_cb)
{
    g_fileOpen  = open_cb;
    g_fileClose = close_cb;
    g_fileSeek  = seek_cb;
    g_fileRead  = read_cb;
}

// --- Driver info ------------------------------------------------------------

void AIL_get_DirectSound_info(HDIGDRIVER, void** ppds, void** ppbuf)
    { if(ppds) *ppds=NULL; if(ppbuf) *ppbuf=NULL; }

// --- 2-D sample management -------------------------------------------------

HSAMPLE AIL_allocate_sample_handle(HDIGDRIVER)
{
    if (!g_context) return NULL;
    OALSample* s = new OALSample();
    alGenSources(1, &s->src);
    alGenBuffers(1, &s->buf);
    // 2D: source relative to listener, panning handled by position
    alSourcei(s->src, AL_SOURCE_RELATIVE, AL_TRUE);
    return (HSAMPLE)s;
}

void AIL_release_sample_handle(HSAMPLE hs)
{
    OALSample* s = (OALSample*)hs;
    if (!s) return;
    alSourceStop(s->src);
    alSourcei(s->src, AL_BUFFER, 0);
    alDeleteSources(1, &s->src);
    alDeleteBuffers(1, &s->buf);
    delete s;
}

void AIL_init_sample(HSAMPLE hs)
{
    OALSample* s = (OALSample*)hs;
    if (!s) return;
    alSourceStop(s->src);
    alSourcei(s->src, AL_BUFFER, 0);
    s->eosCallback = NULL;
    s->volume = 1.f; s->pan = 0.f; s->playing = false;
}

void AIL_set_sample_file(HSAMPLE hs, void const* data, AILSOUNDINFO* /*info*/)
{
    OALSample* s = (OALSample*)hs;
    if (!s || !data) return;

    WORD  fmt = 0, ch = 0, bits = 0, block = 0;
    DWORD rate = 0, dataLen = 0;
    const void* pcmData = NULL;

    // data must be a complete WAV (including header from AIL_decompress_ADPCM)
    if (!parseWav(data, 0x7FFFFFFF, &fmt, &ch, &rate, &bits, &block, &pcmData, &dataLen))
        return;

    ALenum alFmt = oalFormat(ch, bits);
    alSourceStop(s->src);
    alSourcei(s->src, AL_BUFFER, 0);
    alBufferData(s->buf, alFmt, pcmData, (ALsizei)dataLen, (ALsizei)rate);
    alSourcei(s->src, AL_BUFFER, (ALint)s->buf);
    applyVolPan(s->src, s->volume, s->pan);
}

void AIL_start_sample(HSAMPLE hs)
{
    OALSample* s = (OALSample*)hs;
    if (!s) return;
    alSourcePlay(s->src);
    s->playing = true;
}

void AIL_stop_sample(HSAMPLE hs)
{
    OALSample* s = (OALSample*)hs;
    if (!s) return;
    alSourceStop(s->src);
    s->playing = false;
    if (s->eosCallback) s->eosCallback(hs);
}

void AIL_resume_sample(HSAMPLE hs)
{
    OALSample* s = (OALSample*)hs;
    if (!s) return;
    ALint state;
    alGetSourcei(s->src, AL_SOURCE_STATE, &state);
    if (state == AL_PAUSED) alSourcePlay(s->src);
}

void AIL_set_sample_volume_pan(HSAMPLE hs, F32 vol, F32 pan)
{
    OALSample* s = (OALSample*)hs;
    if (!s) return;
    s->volume = vol; s->pan = pan;
    applyVolPan(s->src, vol, pan);
}

void AIL_sample_volume_pan(HSAMPLE hs, F32* vol, F32* pan)
{
    OALSample* s = (OALSample*)hs;
    if (!s) { if(vol)*vol=1.f; if(pan)*pan=0.f; return; }
    if (vol) *vol = s->volume;
    if (pan) *pan = s->pan;
}

void AIL_set_sample_playback_rate(HSAMPLE hs, S32 rate)
{
    OALSample* s = (OALSample*)hs;
    if (!s || rate <= 0) return;
    // Express as pitch ratio against the buffer's native rate
    ALint bufRate = 0;
    alGetBufferi(s->buf, AL_FREQUENCY, &bufRate);
    if (bufRate > 0)
        alSourcef(s->src, AL_PITCH, (float)rate / (float)bufRate);
}

S32 AIL_sample_playback_rate(HSAMPLE hs)
{
    OALSample* s = (OALSample*)hs;
    if (!s) return 22050;
    ALint bufRate = 22050;
    alGetBufferi(s->buf, AL_FREQUENCY, &bufRate);
    ALfloat pitch = 1.f;
    alGetSourcef(s->src, AL_PITCH, &pitch);
    return (S32)((float)bufRate * pitch);
}

void AIL_set_sample_processor(HSAMPLE, U32, HPROVIDER)   {}
void AIL_set_filter_sample_preference(HSAMPLE, char const*, void const*) {}

void* AIL_sample_user_data(HSAMPLE hs, U32 index)
{
    OALSample* s = (OALSample*)hs;
    if (!s || index >= 8) return NULL;
    return s->userData[index];
}

void AIL_set_sample_user_data(HSAMPLE hs, U32 index, void* val)
{
    OALSample* s = (OALSample*)hs;
    if (!s || index >= 8) return;
    s->userData[index] = val;
}

void AIL_set_sample_user_data(HSAMPLE hs, U32 index, int val)
{
    OALSample* s = (OALSample*)hs;
    if (!s || index >= 8) return;
    s->userData[index] = (void*)(intptr_t)val;
}

void AIL_register_EOS_callback(HSAMPLE hs, AILSAMPLECB cb)
{
    OALSample* s = (OALSample*)hs;
    if (s) s->eosCallback = cb;
}

// --- 3-D sample management -------------------------------------------------

H3DSAMPLE AIL_allocate_3D_sample_handle(HPROVIDER)
{
    if (!g_context) return NULL;
    OAL3DSample* s = new OAL3DSample();
    alGenSources(1, &s->src);
    alGenBuffers(1, &s->buf);
    alSourcei(s->src, AL_SOURCE_RELATIVE, AL_FALSE);
    return (H3DSAMPLE)s;
}

void AIL_release_3D_sample_handle(H3DSAMPLE hs)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s) return;
    alSourceStop(s->src);
    alSourcei(s->src, AL_BUFFER, 0);
    alDeleteSources(1, &s->src);
    alDeleteBuffers(1, &s->buf);
    delete s;
}

void AIL_set_3D_sample_file(H3DSAMPLE hs, void const* data, AILSOUNDINFO* /*info*/)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s || !data) return;
    WORD  fmt = 0, ch = 0, bits = 0, block = 0;
    DWORD rate = 0, dataLen = 0;
    const void* pcmData = NULL;
    if (!parseWav(data, 0x7FFFFFFF, &fmt, &ch, &rate, &bits, &block, &pcmData, &dataLen))
        return;
    ALenum alFmt = oalFormat(ch, bits);
    alSourceStop(s->src);
    alSourcei(s->src, AL_BUFFER, 0);
    alBufferData(s->buf, alFmt, pcmData, (ALsizei)dataLen, (ALsizei)rate);
    alSourcei(s->src, AL_BUFFER, (ALint)s->buf);
    alSourcef(s->src, AL_GAIN, s->volume);
}

void AIL_set_3D_sample_file(H3DSAMPLE hs, void const* data)
{
    AIL_set_3D_sample_file(hs, data, NULL);
}

void AIL_start_3D_sample(H3DSAMPLE hs)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (s) { alSourcePlay(s->src); s->playing = true; }
}

void AIL_stop_3D_sample(H3DSAMPLE hs)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s) return;
    alSourceStop(s->src);
    s->playing = false;
    if (s->eosCallback) s->eosCallback(hs);
}

void AIL_resume_3D_sample(H3DSAMPLE hs)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s) return;
    ALint state;
    alGetSourcei(s->src, AL_SOURCE_STATE, &state);
    if (state == AL_PAUSED) alSourcePlay(s->src);
}

void AIL_set_3D_sample_volume(H3DSAMPLE hs, F32 vol)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s) return;
    s->volume = vol;
    alSourcef(s->src, AL_GAIN, vol < 0.f ? 0.f : vol);
}

void AIL_set_3D_sample_playback_rate(H3DSAMPLE hs, S32 rate)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s || rate <= 0) return;
    ALint bufRate = 0;
    alGetBufferi(s->buf, AL_FREQUENCY, &bufRate);
    if (bufRate > 0)
        alSourcef(s->src, AL_PITCH, (float)rate / (float)bufRate);
}

S32 AIL_3D_sample_playback_rate(H3DSAMPLE hs)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s) return 22050;
    ALint bufRate = 22050;
    alGetBufferi(s->buf, AL_FREQUENCY, &bufRate);
    ALfloat pitch = 1.f;
    alGetSourcef(s->src, AL_PITCH, &pitch);
    return (S32)((float)bufRate * pitch);
}

void AIL_set_3D_sample_occlusion(H3DSAMPLE hs, F32 occ)
{
    // Map occlusion [0..1] to gain reduction.  OpenAL has no occlusion API;
    // we reduce gain proportionally.
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s) return;
    float reducedVol = s->volume * (1.f - (occ < 0.f ? 0.f : (occ > 1.f ? 1.f : occ)));
    alSourcef(s->src, AL_GAIN, reducedVol < 0.f ? 0.f : reducedVol);
}

void AIL_set_3D_sample_distances(H3DSAMPLE hs, F32 nearDist, F32 farDist)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s) return;
    alSourcef(s->src, AL_REFERENCE_DISTANCE, nearDist);
    alSourcef(s->src, AL_MAX_DISTANCE,       farDist);
}

// AIL_set_3D_position handles both listener (H3DPOBJECT) and sample (H3DSAMPLE)
// objects.  We distinguish by checking the isListener flag of OALListener.
void AIL_set_3D_position(H3DPOBJECT obj, F32 x, F32 y, F32 z)
{
    if (!obj) return;
    OALListener* li = (OALListener*)obj;
    if (li->isListener) {
        alListener3f(AL_POSITION, x, y, z);
    } else {
        OAL3DSample* s = (OAL3DSample*)obj;
        alSource3f(s->src, AL_POSITION, x, y, z);
    }
}

void* AIL_3D_user_data(H3DSAMPLE hs, U32 index)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s || index >= 8) return NULL;
    return s->userData[index];
}

void AIL_set_3D_user_data(H3DSAMPLE hs, U32 index, void* val)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s || index >= 8) return;
    s->userData[index] = val;
}

void AIL_set_3D_user_data(H3DSAMPLE hs, U32 index, int val)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (!s || index >= 8) return;
    s->userData[index] = (void*)(intptr_t)val;
}

void AIL_register_3D_EOS_callback(H3DSAMPLE hs, AIL3DSAMPLECB cb)
{
    OAL3DSample* s = (OAL3DSample*)hs;
    if (s) s->eosCallback = cb;
}

// --- Streaming audio -------------------------------------------------------

HSTREAM AIL_open_stream(HDIGDRIVER, void const* data, S32 /*block_size*/)
{
    if (!g_context || !data) return NULL;
    const char* filename = (const char*)data;

    if (!g_fileOpen || !g_fileRead || !g_fileSeek || !g_fileClose) return NULL;

    U32 fh = 0;
    if (!g_fileOpen(filename, &fh)) return NULL;

    // Read WAV header (first 44 bytes)
    BYTE header[64] = {};
    U32 headerRead = g_fileRead(fh, header, 44);
    if (headerRead < 44) { g_fileClose(fh); return NULL; }

    WORD  fmt = 0, ch = 0, bits = 0, block = 0;
    DWORD rate = 0, dataLen = 0;
    const void* dataDummy = NULL;
    if (!parseWav(header, 44, &fmt, &ch, &rate, &bits, &block, &dataDummy, &dataLen)) {
        g_fileClose(fh);
        return NULL;
    }
    // Only handle PCM streams for now; ADPCM streams would require decode in thread
    if (fmt != 1) { g_fileClose(fh); return NULL; }

    OALStream* s = new OALStream();
    strncpy_s(s->filename, sizeof(s->filename), filename, _TRUNCATE);
    s->fileHandle    = fh;
    s->channels      = ch;
    s->sampleRate    = rate;
    s->bitsPerSample = bits;

    alGenSources(1, &s->src);
    alGenBuffers(OAL_STREAM_BUFFERS, s->bufs);
    alSourcei(s->src, AL_SOURCE_RELATIVE, AL_TRUE);
    applyVolPan(s->src, s->volume, s->pan);

    // Prime both buffers
    ALenum alFmt = oalFormat(ch, bits);
    BYTE* primeBuf = (BYTE*)malloc(OAL_STREAM_BUFSIZE);
    if (primeBuf) {
        for (int i = 0; i < OAL_STREAM_BUFFERS; ++i) {
            U32 got = g_fileRead(s->fileHandle, primeBuf, OAL_STREAM_BUFSIZE);
            if (got > 0) {
                alBufferData(s->bufs[i], alFmt, primeBuf, (ALsizei)got, (ALsizei)rate);
                alSourceQueueBuffers(s->src, 1, &s->bufs[i]);
            }
        }
        free(primeBuf);
    }

    s->thread = CreateThread(NULL, 0, streamThread, s, CREATE_SUSPENDED, NULL);
    return (HSTREAM)s;
}

void AIL_close_stream(HSTREAM hs)
{
    OALStream* s = (OALStream*)hs;
    if (!s) return;
    EnterCriticalSection(&s->cs);
    s->stopThread = true;
    LeaveCriticalSection(&s->cs);
    if (s->thread) {
        ResumeThread(s->thread);
        WaitForSingleObject(s->thread, 2000);
        CloseHandle(s->thread);
    }
    alSourceStop(s->src);
    alSourcei(s->src, AL_BUFFER, 0);
    alDeleteSources(1, &s->src);
    alDeleteBuffers(OAL_STREAM_BUFFERS, s->bufs);
    if (s->fileHandle && g_fileClose) g_fileClose(s->fileHandle);
    delete s;
}

void AIL_start_stream(HSTREAM hs)
{
    OALStream* s = (OALStream*)hs;
    if (!s) return;
    s->paused = false;
    alSourcePlay(s->src);
    if (s->thread) ResumeThread(s->thread);
}

void AIL_pause_stream(HSTREAM hs, S32 on_off)
{
    OALStream* s = (OALStream*)hs;
    if (!s) return;
    EnterCriticalSection(&s->cs);
    s->paused = (on_off != 0);
    if (s->paused) alSourcePause(s->src);
    else           alSourcePlay(s->src);
    LeaveCriticalSection(&s->cs);
}

void AIL_set_stream_volume_pan(HSTREAM hs, F32 vol, F32 pan)
{
    OALStream* s = (OALStream*)hs;
    if (!s) return;
    s->volume = vol; s->pan = pan;
    applyVolPan(s->src, vol, pan);
}

void AIL_stream_volume_pan(HSTREAM hs, F32* vol, F32* pan)
{
    OALStream* s = (OALStream*)hs;
    if (!s) { if(vol)*vol=1.f; if(pan)*pan=0.f; return; }
    if (vol) *vol = s->volume;
    if (pan) *pan = s->pan;
}

S32 AIL_stream_loop_count(HSTREAM hs)
{
    OALStream* s = (OALStream*)hs;
    return s ? s->loopCount : 0;
}

void AIL_set_stream_loop_count(HSTREAM hs, S32 count)
{
    OALStream* s = (OALStream*)hs;
    if (!s) return;
    s->loopCount = count;
    s->loopsRemaining = count;
}

S32 AIL_stream_ms_position(HSTREAM hs)
{
    OALStream* s = (OALStream*)hs;
    if (!s) return 0;
    ALfloat offset = 0.f;
    alGetSourcef(s->src, AL_SEC_OFFSET, &offset);
    return (S32)(offset * 1000.f);
}

void AIL_stream_ms_position(HSTREAM hs, long* cur, long* total)
{
    OALStream* s = (OALStream*)hs;
    if (!s) { if(cur)*cur=0; if(total)*total=0; return; }
    if (cur)   *cur   = (long)AIL_stream_ms_position(hs);
    if (total) *total = 0;   // total requires knowing file size — not tracked here
}

void AIL_register_stream_callback(HSTREAM hs, AILSTREAMCB cb)
{
    OALStream* s = (OALStream*)hs;
    if (s) s->callback = cb;
}

// --- 3-D provider / listener -----------------------------------------------

S32 AIL_open_3D_provider(HPROVIDER prov)
{
    // prov == 1 is our single "OpenAL Soft" provider
    if (prov != 1) return 0;  // 0 = failure in Miles API
    return (g_context != NULL) ? 1 : 0;
}

void AIL_close_3D_provider(HPROVIDER) {}

H3DPOBJECT AIL_open_3D_listener(HPROVIDER)
{
    // One global listener — return a stable pointer
    static OALListener s_listener;
    return (H3DPOBJECT)&s_listener;
}

void AIL_close_3D_listener(H3DPOBJECT) {}

void AIL_set_3D_speaker_type(HPROVIDER, S32) {}

void AIL_set_3D_orientation(H3DPOBJECT obj,
                             F32 fx, F32 fy, F32 fz,
                             F32 ux, F32 uy, F32 uz)
{
    if (!obj) return;
    OALListener* li = (OALListener*)obj;
    if (li->isListener) {
        float orient[6] = { fx, fy, fz, ux, uy, uz };
        alListenerfv(AL_ORIENTATION, orient);
    }
    // 3D samples don't have orientation in this shim
}

// Enumerate: single "OpenAL Soft" provider with prov=1
S32 AIL_enumerate_3D_providers(HPROVIDER* last, char const** name)
{
    if (!last || !name) return 0;
    if (*last == 0) { *last = 1; *name = "OpenAL Soft"; return 1; }
    *last = 0; *name = NULL;
    return 0;
}

S32 AIL_enumerate_3D_providers(HPROENUM* next, HPROVIDER* prov, char** name)
{
    if (!next || !prov || !name) return 0;
    if (*next == HPROENUM_FIRST) { *next = 1; *prov = 1; *name = (char*)"OpenAL Soft"; return 1; }
    *prov = 0; *name = NULL;
    return 0;
}

// --- Filter enumeration ----------------------------------------------------

S32 AIL_enumerate_filters(void* last, HPROVIDER* prov, char const** name)
    { if(prov)*prov=0; if(name)*name=NULL; return 0; }
S32 AIL_enumerate_filters(HPROENUM* next, HPROVIDER* prov, char** name)
    { if(next)*next=0; if(prov)*prov=0; if(name)*name=NULL; return 0; }

// --- Quick-play helpers ----------------------------------------------------

HAUDIO AIL_quick_load_and_play(char const* path, U32 loop, S32 /*reserve*/)
{
    if (!path || !g_context) return NULL;
    // Quick-play: allocate, load from disk directly, play, return handle
    // We use a static HDIGDRIVER placeholder (value doesn't matter in our shim)
    HSAMPLE s = AIL_allocate_sample_handle((HDIGDRIVER)1);
    if (!s) return NULL;

    // Read file from disk
    FILE* f = fopen(path, "rb");
    if (!f) { AIL_release_sample_handle(s); return NULL; }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz <= 0) { fclose(f); AIL_release_sample_handle(s); return NULL; }
    void* buf = malloc((size_t)sz);
    if (!buf) { fclose(f); AIL_release_sample_handle(s); return NULL; }
    fread(buf, 1, (size_t)sz, f);
    fclose(f);

    AIL_set_sample_file(s, buf, NULL);
    free(buf);

    if (loop) {
        ALuint src = ((OALSample*)s)->src;
        alSourcei(src, AL_LOOPING, AL_TRUE);
    }
    AIL_start_sample(s);
    return (HAUDIO)s;
}

void AIL_quick_unload(void* handle)
{
    if (handle) AIL_release_sample_handle((HSAMPLE)handle);
}

void AIL_quick_set_volume(HAUDIO handle, F32 vol, F32 pan)
{
    if (handle) AIL_set_sample_volume_pan((HSAMPLE)handle, vol, pan);
}

// --- WAV utilities ---------------------------------------------------------

void AIL_WAV_info(void const* data, AILSOUNDINFO* info)
{
    if (!info) return;
    ZeroMemory(info, sizeof(*info));
    if (!data) return;
    WORD  fmt = 0, ch = 0, bits = 0, block = 0;
    DWORD rate = 0, dataLen = 0;
    const void* pcmData = NULL;
    if (!parseWav(data, 0x7FFFFFFF, &fmt, &ch, &rate, &bits, &block, &pcmData, &dataLen))
        return;
    info->format     = fmt;
    info->data_ptr   = (void*)pcmData;
    info->data_len   = dataLen;
    info->rate       = rate;
    info->bits       = bits;
    info->channels   = ch;
    info->block_size = block;
    info->samples    = (bits > 0 && ch > 0) ? (dataLen / (bits/8) / ch) : 0;
    info->initial_ptr = info->data_ptr;
}

void AIL_decompress_ADPCM(AILSOUNDINFO* info, void** out, U32* size)
{
    if (!out) return;
    *out = NULL;
    if (size) *size = 0;
    if (!info || !info->data_ptr || info->format != WAVE_FORMAT_IMA_ADPCM) return;

    WORD  channels   = (WORD)info->channels;
    DWORD rate       = info->rate;
    DWORD blockSize  = info->block_size;
    if (blockSize == 0 || channels == 0 || channels > 2) return;

    const BYTE* src  = (const BYTE*)info->data_ptr;
    DWORD       remaining = info->data_len;

    // Worst-case output: each block of blockSize bytes can yield at most
    // (blockSize - 4*channels) * 2 samples per channel + 1 preamble sample.
    // S16 = 2 bytes per sample.
    DWORD numBlocks  = (remaining + blockSize - 1) / blockSize;
    // Samples per block (mono): 1 preamble + (blockSize-4)*2 nibbles
    DWORD spb        = 1 + (blockSize - 4u * channels) * 2u;
    DWORD totalSamples = numBlocks * spb * channels;

    DWORD pcmBytes = totalSamples * 2u;  // S16
    DWORD hdrBytes = 44u;
    BYTE* outBuf   = (BYTE*)malloc(hdrBytes + pcmBytes);
    if (!outBuf) return;

    // Write PCM WAV header
    writePcmHeader(outBuf, pcmBytes, channels, rate, 16);

    S16*  pcmOut   = (S16*)(outBuf + hdrBytes);
    DWORD actualSamples = 0;

    while (remaining >= 4u) {
        DWORD thisBlock = (remaining < blockSize) ? remaining : blockSize;
        DWORD blockSamples = 0;
        decodeAdpcmBlock(src, thisBlock, channels, pcmOut, &blockSamples);
        pcmOut       += blockSamples;
        actualSamples += blockSamples;
        src           += thisBlock;
        remaining     -= thisBlock;
    }

    // Patch the actual data size into the WAV header
    DWORD actualPcmBytes = actualSamples * 2u;
    writePcmHeader(outBuf, actualPcmBytes, channels, rate, 16);

    *out  = outBuf;
    if (size) *size = hdrBytes + actualPcmBytes;
}

void AIL_mem_free_lock(void* p)
{
    free(p);
}
