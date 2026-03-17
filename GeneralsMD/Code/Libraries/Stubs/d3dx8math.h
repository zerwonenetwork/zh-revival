#ifndef D3DX8MATH_H
#define D3DX8MATH_H

#include "d3d8.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct D3DXMATRIX {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DXMATRIX;

inline D3DXMATRIX* D3DXMatrixInverse(D3DXMATRIX* pOut, float* pDeterminant, const D3DXMATRIX* pM) {
    if (pOut) {
        // Identity matrix fallback stub
        pOut->_11 = 1.0f; pOut->_12 = 0.0f; pOut->_13 = 0.0f; pOut->_14 = 0.0f;
        pOut->_21 = 0.0f; pOut->_22 = 1.0f; pOut->_23 = 0.0f; pOut->_24 = 0.0f;
        pOut->_31 = 0.0f; pOut->_32 = 0.0f; pOut->_33 = 1.0f; pOut->_34 = 0.0f;
        pOut->_41 = 0.0f; pOut->_42 = 0.0f; pOut->_43 = 0.0f; pOut->_44 = 1.0f;
    }
    if (pDeterminant) *pDeterminant = 1.0f;
    return pOut;
}

#ifdef __cplusplus
}
#endif

#endif
