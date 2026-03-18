// srVector2.hpp — ZH-REVIVAL STUB
// Minimal srVector2 type for Surrender engine integration.
#pragma once
#ifndef SR_VECTOR2_HPP
#define SR_VECTOR2_HPP

struct srVector2 {
    float x, y;
    srVector2() : x(0), y(0) {}
    srVector2(float _x, float _y) : x(_x), y(_y) {}
};

#endif // SR_VECTOR2_HPP
