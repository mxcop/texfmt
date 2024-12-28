#pragma once

#include <cmath>

#include "type.hh"
#include "math/clamp.hh"

// /* Source: <https://gamedev.stackexchange.com/questions/17326/conversion-of-a-number-from-single-precision-floating-point-representation-to-a> */
// inline u16 f32tof16(const f32 x) {
//     u16 bits = ((u32&)x >> 16) & 0x8000;
//     u16 m = ((u32&)x >> 12) & 0x07ff;
//     const u32 e = ((u32&)x >> 23) & 0xff;

//     if (e < 103) return bits;

//     if (e > 142) {
//         bits |= 0x7c00u;
//         bits |= e == 255 && ((u32&)x & 0x007fffffu);
//         return bits;
//     }

//     if (e < 113) {
//         m |= 0x0800u;
//         bits |= (m >> (114 - e)) + ((m >> (113 - e)) & 1);
//         return bits;
//     }

//     bits |= ((e - 112) << 10) | (m >> 1);
//     bits += m & 1;
//     return bits;
// }

inline bool isfinite(const f32 x) {
    return isnan(x) ? 0 : !isinf(x);
}

#include <immintrin.h>

u16 f32tof16(const f32 f) {
    // if (!isfinite(f)) return 0u;

    // __m128i dst;
    // _mm_storeu_si128(&dst, _mm256_cvtps_ph(_mm256_loadu_ps(&f), 0));
    // return (u16&)dst;

    u32 x = (u32&)f;
    u16 h = ((x>>16)&0x8000)|((((x&0x7f800000)-0x38000000)>>13)&0x7c00)|((x>>13)&0x03ff);
    return h;
}

f32 f16tof32(const u16 f) {
    u32 r = (((u32)f&0x8000)<<16) | ((((u32)f&0x7c00)+0x1C000)<<13) | (((u32)f&0x03FF)<<13);
    return (f32&)r;

    // const u32 exp = f & 0b111111;
    // const u32 man = (u32)(f & 0b1111111111000000) << 3u;
    // const u32 val = (exp | man);
    // return (f32&)val;
}

/* Encode a HDR color value packed as R11G11B10. */
u32 encode_r11g11b10(v3 rgb) {
    /* Avoid accidentally rounding up to infinity */
    const f32 sr = min(rgb.x, 64512.0f);
    const f32 sg = min(rgb.y, 64512.0f);
    const f32 sb = min(rgb.z, 64512.0f);
    
    const u32 r = (((u32)f32tof16(sr) + 8u) >> 4u) & 0x000007FFu;
    const u32 g = (((u32)f32tof16(sg) + 8u) << 7u) & 0x003FF800u;
    const u32 b = (((u32)f32tof16(sb) + 16u) << 17u) & 0xFFC00000u;

    return r | g | b;
}

/* Decode a HDR color value packed as R11G11B10. */
v3 decode_r11g11b10(const u32 p) {
    const f32 r = f16tof32((p << 4u ) & 0x7FF0u);
    const f32 g = f16tof32((p >> 7u ) & 0x7FF0u);
    const f32 b = f16tof32((p >> 17u) & 0x7FE0u);
    return { r, g, b };
}
