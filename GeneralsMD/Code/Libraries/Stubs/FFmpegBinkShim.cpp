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
// P5-02 — FFmpeg Bink Video Shim
//
// Implements every Bink* function declared (not defined) in BinkVideoStub.h
// when ZH_FFMPEG_VIDEO is defined.  BinkVideoPlayer.cpp is unchanged; it
// continues to call BinkOpen/BinkDoFrame/BinkCopyToBuffer etc., which now
// dispatch to FFmpeg libavcodec + libswscale.
//
// FFmpeg's Bink decoder (bink/binkdec) is a clean-room reverse-engineering
// included in FFmpeg since 2011 and distributed under the LGPL 2.1+.
// This project is GPLv3, which is compatible with LGPL 2.1+.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
//----------------------------------------------------------------------------

#define STUB_IMPL
#define ZH_FFMPEG_VIDEO
#include "BinkVideoStub.h"  // extern declarations only in this mode

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <cstdio>
#include <cstring>

//----------------------------------------------------------------------------
// Internal logging
//----------------------------------------------------------------------------

static void binkLog(const char* fmt, ...)
{
    FILE* f = fopen("Logs/video_skip.log", "a");
    if (!f) return;
    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);
    fclose(f);
}

//----------------------------------------------------------------------------
// Map BINKSURFACE* flags to AV pixel formats for sws_scale output
//----------------------------------------------------------------------------

static AVPixelFormat surfaceFlagToAvFmt(U32 flags)
{
    switch (flags) {
        case BINKSURFACE32:  return AV_PIX_FMT_BGR0;    // 32-bit XRGB → stored BGRX
        case BINKSURFACE24:  return AV_PIX_FMT_BGR24;   // 24-bit RGB
        case BINKSURFACE565: return AV_PIX_FMT_RGB565LE; // 16-bit RGB565
        case BINKSURFACE555: return AV_PIX_FMT_RGB555LE; // 15-bit RGB555
        default:             return AV_PIX_FMT_BGR0;
    }
}

//============================================================================
//  Bink* implementations
//============================================================================

HBINK BinkOpen(char const* path, DWORD /*flags*/)
{
    if (!path) return NULL;

    BINK* b = new BINK();
    memset(b, 0, sizeof(BINK));

    b->_fmtCtx   = NULL;
    b->_codecCtx = NULL;
    b->_frame    = NULL;
    b->_packet   = NULL;
    b->_swsCtx   = NULL;
    b->_videoStream = -1;
    b->_frameReady  = false;

    if (avformat_open_input(&b->_fmtCtx, path, NULL, NULL) < 0) {
        binkLog("P5-02 BinkOpen: avformat_open_input failed: %s\n", path);
        delete b; return NULL;
    }

    if (avformat_find_stream_info(b->_fmtCtx, NULL) < 0) {
        binkLog("P5-02 BinkOpen: avformat_find_stream_info failed: %s\n", path);
        avformat_close_input(&b->_fmtCtx);
        delete b; return NULL;
    }

    // Find the first video stream
    for (unsigned i = 0; i < b->_fmtCtx->nb_streams; ++i) {
        if (b->_fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            b->_videoStream = (int)i;
            break;
        }
    }
    if (b->_videoStream < 0) {
        binkLog("P5-02 BinkOpen: no video stream in: %s\n", path);
        avformat_close_input(&b->_fmtCtx);
        delete b; return NULL;
    }

    AVStream* vs = b->_fmtCtx->streams[b->_videoStream];
    const AVCodec* codec = avcodec_find_decoder(vs->codecpar->codec_id);
    if (!codec) {
        binkLog("P5-02 BinkOpen: no decoder for codec %d in: %s\n",
                (int)vs->codecpar->codec_id, path);
        avformat_close_input(&b->_fmtCtx);
        delete b; return NULL;
    }

    b->_codecCtx = avcodec_alloc_context3(codec);
    if (!b->_codecCtx) {
        avformat_close_input(&b->_fmtCtx); delete b; return NULL;
    }
    avcodec_parameters_to_context(b->_codecCtx, vs->codecpar);
    if (avcodec_open2(b->_codecCtx, codec, NULL) < 0) {
        binkLog("P5-02 BinkOpen: avcodec_open2 failed: %s\n", path);
        avcodec_free_context(&b->_codecCtx);
        avformat_close_input(&b->_fmtCtx);
        delete b; return NULL;
    }

    b->_frame  = av_frame_alloc();
    b->_packet = av_packet_alloc();
    if (!b->_frame || !b->_packet) {
        av_frame_free(&b->_frame);
        av_packet_free(&b->_packet);
        avcodec_free_context(&b->_codecCtx);
        avformat_close_input(&b->_fmtCtx);
        delete b; return NULL;
    }

    // Fill public BINK fields
    b->Width        = (DWORD)b->_codecCtx->width;
    b->Height       = (DWORD)b->_codecCtx->height;
    b->FrameNum     = 1;
    b->_frameReady  = false;

    // Total frames: use nb_frames if available, else estimate from duration
    if (vs->nb_frames > 0) {
        b->Frames = (DWORD)vs->nb_frames;
    } else if (vs->duration > 0 && vs->r_frame_rate.den > 0) {
        double fps = (double)vs->r_frame_rate.num / vs->r_frame_rate.den;
        double dur = (double)vs->duration * av_q2d(vs->time_base);
        b->Frames = (DWORD)(fps * dur + 0.5);
    } else {
        b->Frames = 1;
    }
    b->LastFrameNum = b->Frames;

    if (vs->r_frame_rate.den > 0) {
        b->FrameRate    = (DWORD)vs->r_frame_rate.num;
        b->FrameRateDiv = (DWORD)vs->r_frame_rate.den;
    } else {
        b->FrameRate    = 15;
        b->FrameRateDiv = 1;
    }

    return b;
}

void BinkClose(HBINK bink)
{
    if (!bink) return;
    if (bink->_swsCtx)   sws_freeContext(bink->_swsCtx);
    if (bink->_frame)    av_frame_free(&bink->_frame);
    if (bink->_packet)   av_packet_free(&bink->_packet);
    if (bink->_codecCtx) avcodec_free_context(&bink->_codecCtx);
    if (bink->_fmtCtx)   avformat_close_input(&bink->_fmtCtx);
    delete bink;
}

// BinkWait: return 0 (frame ready) or 1 (not ready).
// We decode eagerly in BinkDoFrame, so we always report ready here.
int BinkWait(HBINK bink)
{
    (void)bink;
    return 0;
}

// BinkDoFrame: decode the next frame from the stream.
void BinkDoFrame(HBINK bink)
{
    if (!bink || !bink->_fmtCtx) return;
    bink->_frameReady = false;

    // Read packets until we get a decoded video frame
    while (av_read_frame(bink->_fmtCtx, bink->_packet) >= 0) {
        if (bink->_packet->stream_index != bink->_videoStream) {
            av_packet_unref(bink->_packet);
            continue;
        }
        if (avcodec_send_packet(bink->_codecCtx, bink->_packet) < 0) {
            av_packet_unref(bink->_packet);
            continue;
        }
        av_packet_unref(bink->_packet);
        if (avcodec_receive_frame(bink->_codecCtx, bink->_frame) == 0) {
            bink->_frameReady = true;
            return;
        }
    }
    // EOF: flush the decoder
    avcodec_send_packet(bink->_codecCtx, NULL);
    if (avcodec_receive_frame(bink->_codecCtx, bink->_frame) == 0)
        bink->_frameReady = true;
}

// BinkCopyToBuffer: blit the decoded frame into the caller's pixel buffer.
void BinkCopyToBuffer(HBINK bink, void* dest, S32 destpitch,
                      U32 destheight, U32 destx, U32 desty, U32 flags)
{
    if (!bink || !dest || !bink->_frameReady) return;

    int srcW = bink->_codecCtx->width;
    int srcH = bink->_codecCtx->height;
    AVPixelFormat dstFmt = surfaceFlagToAvFmt(flags);

    // (Re-)create the sws context if format/size changed
    bink->_swsCtx = sws_getCachedContext(
        bink->_swsCtx,
        srcW, srcH, bink->_codecCtx->pix_fmt,
        srcW, srcH, dstFmt,
        SWS_BILINEAR, NULL, NULL, NULL);
    if (!bink->_swsCtx) return;

    // Compute bytes-per-pixel for the destination format
    int dstBpp;
    switch (flags) {
        case BINKSURFACE32:  dstBpp = 4; break;
        case BINKSURFACE24:  dstBpp = 3; break;
        case BINKSURFACE565:
        case BINKSURFACE555: dstBpp = 2; break;
        default:             dstBpp = 4; break;
    }

    // Offset destination pointer by (destx, desty)
    BYTE* dstPtr  = (BYTE*)dest + (desty * (U32)destpitch) + (destx * (U32)dstBpp);
    int   dstPitch = destpitch;

    // Clamp blit height to what's available in the destination
    int blitH = srcH;
    if ((U32)blitH > destheight - desty) blitH = (int)(destheight - desty);

    uint8_t* dstPlanes[4]   = { dstPtr, NULL, NULL, NULL };
    int      dstStrides[4]  = { dstPitch, 0, 0, 0 };

    sws_scale(bink->_swsCtx,
              (const uint8_t* const*)bink->_frame->data,
              bink->_frame->linesize,
              0, blitH,
              dstPlanes, dstStrides);
}

// BinkNextFrame: advance frame counter; next BinkDoFrame reads the next packet.
void BinkNextFrame(HBINK bink)
{
    if (!bink) return;
    bink->FrameNum++;
    bink->_frameReady = false;
}

// BinkGoto: seek to a specific frame (0-based index).
void BinkGoto(HBINK bink, DWORD frame, int /*flags*/)
{
    if (!bink || !bink->_fmtCtx || bink->_videoStream < 0) return;
    AVStream* vs = bink->_fmtCtx->streams[bink->_videoStream];
    // Convert frame index to timestamp in stream timebase
    int64_t ts = (vs->r_frame_rate.num > 0)
        ? av_rescale_q((int64_t)frame,
                       av_inv_q(vs->r_frame_rate),
                       vs->time_base)
        : (int64_t)frame;
    av_seek_frame(bink->_fmtCtx, bink->_videoStream, ts, AVSEEK_FLAG_BACKWARD);
    avcodec_flush_buffers(bink->_codecCtx);
    bink->FrameNum    = frame + 1;
    bink->_frameReady = false;
}

DWORD BinkGetFrameNum(HBINK bink)
{
    return bink ? bink->FrameNum : 0;
}

void BinkPause(HBINK /*bink*/, int /*pause*/) {}

// Audio integration: not applicable when audio goes through OpenAL (P5-01).
void BinkSetSoundSystem(void* /*open_func*/, DWORD /*driver*/) {}
void BinkSetVolume(HBINK /*bink*/, U32 /*track*/, S32 /*volume*/) {}
void BinkSetSoundTrack(U32 /*count*/, U32* /*tracks*/) {}
int  BinkSoundUseDirectSound(void* /*driver*/) { return 1; }
