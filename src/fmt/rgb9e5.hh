#pragma once

#include <cmath>

#include "type.hh"
#include "math/clamp.hh"

/* Encode a HDR color value packed as RGB9E5. */
u32 encode_rgb9e5(v3 rgb) {
    const f32 kmax = 65408.0f; /* 1.FF x 2^+15 */
    const f32 kmin = .000015f; /* 1.00 x 2^-16 */

    /* Clamp the RGB values */
    const f32 sr = clamp(rgb.x, kmin, kmax);
    const f32 sg = clamp(rgb.y, kmin, kmax);
    const f32 sb = clamp(rgb.z, kmin, kmax);

    /* Find the maximum channel */
    const f32 maxc = max(max(kmin, sr), max(sg, sb));
    const u32 bias = ((u32&)maxc + 0x07804000u) & 0x7F800000u;
    const f32 fbias = (f32&)bias;

    /* Finally shift the bits into their places */
    const f32 pr = sr + fbias;
    const f32 pg = sg + fbias;
    const f32 pb = sb + fbias;
    const u32 pe = (bias << 4u) + 0x10000000u;
    return pe | ((u32&)pb << 18u) | ((u32&)pg << 9u) | ((u32&)pr & 0x1FFu);
}

/* Decode a HDR color value packed as RGB9E5. */
v3 decode_rgb9e5(const u32 p) {
    const f32 x = p & 0x1FFu, y = (p >> 9u) & 0x1FFu, z = (p >> 18u) & 0x1FFu;
    const s32 e = (s32)(p >> 27u) - 24;
    const f32 r = ldexp(x, e), g = ldexp(y, e), b = ldexp(z, e);
    return { r, g, b };
}
