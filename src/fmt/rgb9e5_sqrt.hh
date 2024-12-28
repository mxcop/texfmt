#pragma once

#include <cmath>

#include "type.hh"
#include "math/clamp.hh"

/* Encode a HDR color value packed as RGB9E5. */
u32 encode_rgb9e5_sqrt(v3 rgb) {
    const f32 kmax = 65408.0f; /* 1.FF x 2^+15 */
    const f32 kmin = .000015f; /* 1.00 x 2^-16 */

    /* Clamp the RGB values */
    const f32 sr = clamp(rgb.x, kmin, kmax);
    const f32 sg = clamp(rgb.y, kmin, kmax);
    const f32 sb = clamp(rgb.z, kmin, kmax);

    /* Find the maximum channel */
    const f32 maxc = max(max(kmin, sr), max(sg, sb));
    const u32 scale = (0x7EFFFFFFu - (u32&)maxc) & 0x7F800000u;
    const f32 fscale = (f32&)scale;

    /* Finally shift the bits into their places */
    const f32 pr = sqrt(sr * fscale) * 511.0 + 0.5;
    const f32 pg = sqrt(sg * fscale) * 511.0 + 0.5;
    const f32 pb = sqrt(sb * fscale) * 511.0 + 0.5;
    const u32 pe = (0x47000000 - scale) << 4u;
    return pe | (u32)pb << 18u | (u32)pg << 9u | (u32)pr;
}

/* Decode a HDR color value packed as RGB9E5. */
v3 decode_rgb9e5_sqrt(const u32 p) {
    const f32 x = (f32)(p & 0x1FF) / 511.0f, y = (f32)((p >> 9u) & 0x1FF) / 511.0f, z = (f32)((p >> 18u) & 0x1FF) / 511.0f;
    const s32 e = (s32)(p >> 27u) - 15;
    const f32 r = ldexp(x * x, e), g = ldexp(y * y, e), b = ldexp(z * z, e);
    return { r, g, b };

    // float3 rgb = (uint3(p, p >> 9, p >> 18) & 0x1FF) / 511.0;
    // return ldexp(rgb * rgb, (int)(p >> 27) - 15);
}
