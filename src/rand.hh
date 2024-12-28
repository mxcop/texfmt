#pragma once

#include "type.hh"

inline u32 random_u32(u32& seed) {
    seed ^= seed << 13u;
    seed ^= seed >> 17u;
    seed ^= seed << 5u;
    return seed;
}

inline float random_f32(u32& seed) { return random_u32(seed) * 2.3283064365387e-10f; }
