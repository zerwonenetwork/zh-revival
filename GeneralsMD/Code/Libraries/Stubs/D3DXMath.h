// D3DXMath.h — ZH-REVIVAL STUB
// Resolves: #include "D3DXMath.h" in pointgr.cpp and sortingrenderer.cpp.
// Provides D3DX vector/matrix math types and stub functions.
// Real implementations would use the DX8 SDK d3dx8math.h.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
#pragma once
#ifndef D3DXMATH_H_ZH_STUB
#define D3DXMATH_H_ZH_STUB

#include "d3d8.h"
#include <math.h>

#ifdef __cplusplus

// ---------------------------------------------------------------------------
// D3DX Vector / Matrix types
// ---------------------------------------------------------------------------

typedef struct D3DXVECTOR2 {
    float x, y;
    D3DXVECTOR2() {}
    D3DXVECTOR2(float _x, float _y) : x(_x), y(_y) {}
} D3DXVECTOR2;

typedef struct D3DXVECTOR3 {
    float x, y, z;
    D3DXVECTOR3() {}
    D3DXVECTOR3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    float& operator[](int i) { return (&x)[i]; }
    const float& operator[](int i) const { return (&x)[i]; }
} D3DXVECTOR3;

typedef struct D3DXVECTOR4 {
    float x, y, z, w;
    D3DXVECTOR4() {}
    D3DXVECTOR4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    float& operator[](int i) { return (&x)[i]; }
    const float& operator[](int i) const { return (&x)[i]; }
} D3DXVECTOR4;

typedef struct D3DXMATRIX {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
    D3DXMATRIX() {}
    D3DXMATRIX(
        float a11, float a12, float a13, float a14,
        float a21, float a22, float a23, float a24,
        float a31, float a32, float a33, float a34,
        float a41, float a42, float a43, float a44)
    {
        _11 = a11; _12 = a12; _13 = a13; _14 = a14;
        _21 = a21; _22 = a22; _23 = a23; _24 = a24;
        _31 = a31; _32 = a32; _33 = a33; _34 = a34;
        _41 = a41; _42 = a42; _43 = a43; _44 = a44;
    }
    // Multiply two matrices
    D3DXMATRIX operator*(const D3DXMATRIX& rhs) const {
        D3DXMATRIX result;
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c) {
                result.m[r][c] = 0.0f;
                for (int k = 0; k < 4; ++k)
                    result.m[r][c] += m[r][k] * rhs.m[k][c];
            }
        return result;
    }
    D3DXMATRIX& operator*=(const D3DXMATRIX& rhs) {
        *this = *this * rhs;
        return *this;
    }
} D3DXMATRIX;

#ifndef D3DX_PI
#define D3DX_PI 3.14159265358979323846f
#endif

// ---------------------------------------------------------------------------
// D3DX math functions (stubs)
// ---------------------------------------------------------------------------

// Transform a 3-component vector by a 4x4 matrix, producing a 4-component result.
inline D3DXVECTOR4* D3DXVec3Transform(
    D3DXVECTOR4*       pOut,
    const D3DXVECTOR3* pV,
    const D3DXMATRIX*  pM)
{
    if (!pOut || !pV || !pM) return pOut;
    float x = pV->x, y = pV->y, z = pV->z;
    pOut->x = x*pM->_11 + y*pM->_21 + z*pM->_31 + pM->_41;
    pOut->y = x*pM->_12 + y*pM->_22 + z*pM->_32 + pM->_42;
    pOut->z = x*pM->_13 + y*pM->_23 + z*pM->_33 + pM->_43;
    pOut->w = x*pM->_14 + y*pM->_24 + z*pM->_34 + pM->_44;
    return pOut;
}

// Dot product of two 4-component vectors.
inline float D3DXVec4Dot(const D3DXVECTOR4* pV1, const D3DXVECTOR4* pV2)
{
    if (!pV1 || !pV2) return 0.0f;
    return pV1->x*pV2->x + pV1->y*pV2->y + pV1->z*pV2->z + pV1->w*pV2->w;
}

// Transform a 4-component vector by a matrix.
inline D3DXVECTOR4* D3DXVec4Transform(
    D3DXVECTOR4*       pOut,
    const D3DXVECTOR4* pV,
    const D3DXMATRIX*  pM)
{
    if (!pOut || !pV || !pM) return pOut;
    float x = pV->x, y = pV->y, z = pV->z, w = pV->w;
    pOut->x = x*pM->_11 + y*pM->_21 + z*pM->_31 + w*pM->_41;
    pOut->y = x*pM->_12 + y*pM->_22 + z*pM->_32 + w*pM->_42;
    pOut->z = x*pM->_13 + y*pM->_23 + z*pM->_33 + w*pM->_43;
    pOut->w = x*pM->_14 + y*pM->_24 + z*pM->_34 + w*pM->_44;
    return pOut;
}

// Invert a matrix (stub: returns identity on failure).
inline D3DXMATRIX* D3DXMatrixInverse(
    D3DXMATRIX*       pOut,
    float*            pDeterminant,
    const D3DXMATRIX* pM)
{
    (void)pM;
    if (pOut) {
        D3DXMATRIX identity = {};
        identity._11 = identity._22 = identity._33 = identity._44 = 1.0f;
        *pOut = identity;
    }
    if (pDeterminant) *pDeterminant = 1.0f;
    return pOut;
}

// Return the number of bytes per vertex for an FVF descriptor.
inline UINT D3DXGetFVFVertexSize(DWORD FVF)
{
    UINT size = 0;
    // Position
    if      ((FVF & 0x000E) == 0x000E) size += 5 * sizeof(float); // XYZB4
    else if ((FVF & 0x000E) == 0x0002) size += 3 * sizeof(float); // XYZ
    // Normal
    if (FVF & 0x0010) size += 3 * sizeof(float);
    // Diffuse
    if (FVF & 0x0040) size += sizeof(DWORD);
    // Specular
    if (FVF & 0x0080) size += sizeof(DWORD);
    // Texture coordinates (rough estimate)
    int numTex = (FVF & 0x0F00) >> 8;
    size += numTex * 2 * sizeof(float);
    return size;
}

inline D3DXMATRIX* D3DXMatrixRotationZ(D3DXMATRIX* pOut, float angle)
{
    if (!pOut) {
        return pOut;
    }

    const float c = cosf(angle);
    const float s = sinf(angle);
    *pOut = D3DXMATRIX(
        c,  s,  0.0f, 0.0f,
       -s,  c,  0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    return pOut;
}

inline D3DXMATRIX* D3DXMatrixScaling(D3DXMATRIX* pOut, float sx, float sy, float sz)
{
    if (!pOut) {
        return pOut;
    }

    *pOut = D3DXMATRIX(
        sx,   0.0f, 0.0f, 0.0f,
        0.0f, sy,   0.0f, 0.0f,
        0.0f, 0.0f, sz,   0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    return pOut;
}

inline D3DXMATRIX* D3DXMatrixTranslation(D3DXMATRIX* pOut, float tx, float ty, float tz)
{
    if (!pOut) {
        return pOut;
    }

    *pOut = D3DXMATRIX(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        tx,   ty,   tz,   1.0f);
    return pOut;
}

inline D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX* pOut, const D3DXMATRIX* pM1, const D3DXMATRIX* pM2)
{
    if (!pOut || !pM1 || !pM2) {
        return pOut;
    }

    *pOut = (*pM1) * (*pM2);
    return pOut;
}

inline D3DXMATRIX* D3DXMatrixTranspose(D3DXMATRIX* pOut, const D3DXMATRIX* pM)
{
    if (!pOut || !pM) {
        return pOut;
    }

    D3DXMATRIX result;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            result.m[r][c] = pM->m[c][r];
        }
    }

    *pOut = result;
    return pOut;
}

#endif // __cplusplus

#endif // D3DXMATH_H_ZH_STUB
