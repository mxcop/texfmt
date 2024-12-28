#pragma once

#include "type.hh"

inline f32 clamp(const f32 x, const f32 min, const f32 max) {
    return (x < min) ? min : (x > max) ? max : x;
}

inline f32 max(const f32 x, const f32 y) {
    return x > y ? x : y;
}

inline f32 min(const f32 x, const f32 y) {
    return x < y ? x : y;
}
