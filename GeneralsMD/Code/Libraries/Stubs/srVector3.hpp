// srVector3.hpp — ZH-REVIVAL STUB
// Minimal srVector3 type for Surrender engine integration.
#pragma once
#ifndef SR_VECTOR3_HPP
#define SR_VECTOR3_HPP

struct srVector3 {
    float x, y, z;
    srVector3() : x(0), y(0), z(0) {}
    srVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

#endif // SR_VECTOR3_HPP
