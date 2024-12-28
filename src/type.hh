#pragma once

#include <cstdint>

/* Unsigned integers (u) */
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

/* Signed integers (s) */
using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

/* Floating point numbers (f) */
using f32 = float;
using f64 = double;

/* Floating point vectors (vN) */
union v2 { struct { f32 x, y; }; f32 n[2]; };
union v3 { struct { f32 x, y, z; }; f32 n[3]; };
union v4 { struct { f32 x, y, z, w; }; f32 n[4]; };
