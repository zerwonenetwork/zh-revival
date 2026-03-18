// srMatrix4x3.hpp — ZH-REVIVAL STUB
// Minimal srMatrix4x3 type for Surrender engine integration.
// The Surrender (sr) engine uses this 4x3 (affine) matrix type.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
#pragma once
#ifndef SR_MATRIX4X3_HPP
#define SR_MATRIX4X3_HPP

struct srMatrix4x3 {
    float m[4][3];
    srMatrix4x3() {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 3; ++j)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }
};

#endif // SR_MATRIX4X3_HPP
