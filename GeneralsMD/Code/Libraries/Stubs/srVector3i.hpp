// srVector3i.hpp — ZH-REVIVAL STUB
// Provides a minimal srVector3i struct for the Surrender engine integration.
// The real Surrender SDK headers are not available; these stubs provide the
// minimum type definitions needed for sr_util.h to compile.
#pragma once
#ifndef SR_VECTOR3I_HPP
#define SR_VECTOR3I_HPP

struct srVector3i {
    int x, y, z;
    srVector3i() : x(0), y(0), z(0) {}
    srVector3i(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
};

#endif // SR_VECTOR3I_HPP
