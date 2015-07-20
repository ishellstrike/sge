// Copyright (C) 2011 Giliam de Carpentier - All rights reserved
// Licensed under the Simplified BSD license
// http://www.decarpentier.nl/

uniform sampler2D samplerPerlinPerm2D;
uniform sampler2D samplerPerlinGrad2D;

const float pi = 3.14159265358;
#define saturate(x) clamp(x, 0.0, 1.0)

float perlinNoise(vec2 p, float seed)
{
    // Calculate 2D integer coordinates i and fraction p.
    vec2 i = floor(p);
    vec2 f = p - i;

    // Get weights from the coordinate fraction
    vec2 w = f * f * f * (f * (f * 6 - 15) + 10);
    vec4 w4 = vec4(1, w.x, w.y, w.x * w.y);

    // Get the four randomly permutated indices from the noise lattice nearest to
    // p and offset these numbers with the seed number.
    vec4 perm = texture2D(samplerPerlinPerm2D, i / 256) + seed;

    // Permutate the four offseted indices again and get the 2D gradient for each
    // of the four permutated coordinates-seed pairs.
    vec4 g1 = texture2D(samplerPerlinGrad2D, perm.xy) * 2 - 1;
    vec4 g2 = texture2D(samplerPerlinGrad2D, perm.zw) * 2 - 1;

    // Evaluate the four lattice gradients at p
    float a = dot(g1.xy, f);
    float b = dot(g2.xy, f + vec2(-1,  0));
    float c = dot(g1.zw, f + vec2( 0, -1));
    float d = dot(g2.zw, f + vec2(-1, -1));

    // Bi-linearly blend between the gradients, using w4 as blend factors.
    vec4 grads = vec4(a, b - a, c - a, a - b - c + d);
    float n = dot(grads, w4);

    // Return the noise value, roughly normalized in the range [-1, 1]
    return n * 1.5;
}

vec3 perlinNoiseDeriv(vec2 p, float seed)
{
    // Calculate 2D integer coordinates i and fraction p.
    vec2 i = floor(p);
    vec2 f = p - i;

    // Get weights from the coordinate fraction
    vec2 w = f * f * f * (f * (f * 6 - 15) + 10); // 6f^5 - 15f^4 + 10f^3
    vec4 w4 = vec4(1, w.x, w.y, w.x * w.y);

    // Get the derivative dw/df
    vec2 dw = f * f * (f * (f * 30 - 60) + 30); // 30f^4 - 60f^3 + 30f^2

    // Get the derivative d(w*f)/df
    vec2 dwp = f * f * f * (f * (f * 36 - 75) + 40); // 36f^5 - 75f^4 + 40f^3

    // Get the four randomly permutated indices from the noise lattice nearest to
    // p and offset these numbers with the seed number.
    vec4 perm = texture2D(samplerPerlinPerm2D, i / 256) + seed;

    // Permutate the four offseted indices again and get the 2D gradient for each
    // of the four permutated coordinates-seed pairs.
    vec4 g1 = texture2D(samplerPerlinGrad2D, perm.xy) * 2 - 1;
    vec4 g2 = texture2D(samplerPerlinGrad2D, perm.zw) * 2 - 1;

    // Evaluate the four lattice gradients at p
    float a = dot(g1.xy, f);
    float b = dot(g2.xy, f + vec2(-1,  0));
    float c = dot(g1.zw, f + vec2( 0, -1));
    float d = dot(g2.zw, f + vec2(-1, -1));

    // Bi-linearly blend between the gradients, using w4 as blend factors.
    vec4 grads = vec4(a, b - a, c - a, a - b - c + d);
    float n = dot(grads, w4);

    // Calculate the derivatives dn/dx and dn/dy
    float dx = (g1.x + (g1.z-g1.x)*w.y) + ((g2.y-g1.y)*f.y - g2.x +
               ((g1.y-g2.y-g1.w+g2.w)*f.y + g2.x + g1.w - g2.z - g2.w)*w.y)*
               dw.x + ((g2.x-g1.x) + (g1.x-g2.x-g1.z+g2.z)*w.y)*dwp.x;
    float dy = (g1.y + (g2.y-g1.y)*w.x) + ((g1.z-g1.x)*f.x - g1.w + ((g1.x-
               g2.x-g1.z+g2.z)*f.x + g2.x + g1.w - g2.z - g2.w)*w.x)*dw.y +
               ((g1.w-g1.y) + (g1.y-g2.y-g1.w+g2.w)*w.x)*dwp.y;

    // Return the noise value, roughly normalized in the range [-1, 1]
    // Also return the pseudo dn/dx and dn/dy, scaled by the same factor
    return vec3(n, dx, dy) * 1.5;
}

vec3 perlinNoisePseudoDeriv(vec2 p, float seed)
{
    // Calculate 2D integer coordinates i and fraction p.
    vec2 i = floor(p);
    vec2 f = p - i;

    // Get weights from the coordinate fraction
    vec2 w = f * f * f * (f * (f * 6 - 15) + 10);
    vec4 w4 = vec4(1, w.x, w.y, w.x * w.y);

    // Get pseudo derivative weights
    vec2 dw = f * f * (f * (30 * f - 60) + 30);

    // Get the four randomly permutated indices from the noise lattice nearest to
    // p and offset these numbers with the seed number.
    vec4 perm = texture2D(samplerPerlinPerm2D, i / 256) + seed;

    // Permutate the four offseted indices again and get the 2D gradient for each
    // of the four permutated coordinates-seed pairs.
    vec4 g1 = texture2D(samplerPerlinGrad2D, perm.xy) * 2 - 1;
    vec4 g2 = texture2D(samplerPerlinGrad2D, perm.zw) * 2 - 1;

    // Evaluate the four lattice gradients at p
    float a = dot(g1.xy, f);
    float b = dot(g2.xy, f + vec2(-1,  0));
    float c = dot(g1.zw, f + vec2( 0, -1));
    float d = dot(g2.zw, f + vec2(-1, -1));

    // Bi-linearly blend between the gradients, using w4 as blend factors.
    vec4 grads = vec4(a, b - a, c - a, a - b - c + d);
    float n = dot(grads, w4);

    // Calculate pseudo derivates
    float dx = dw.x * (grads.y + grads.w*w.y);
    float dy = dw.y * (grads.z + grads.w*w.x);

    // Return the noise value, roughly normalized in the range [-1, 1]
    // Also return the pseudo dn/dx and dn/dy, scaled by the same factor
    return vec3(n, dx, dy) * 1.5;
}


float swissTurbulence(vec2 p, float seed, int octaves)
{
     float sum = 0;
     float freq = 1.0, amp = 1.0;
     vec2 dsum = vec2(0,0);
     float lacunarity = 2.0;
     float gain = 0.5;
     float warp = 0.15;
     for(int i=0; i < octaves; i++)
     {
         vec3 n = perlinNoiseDeriv((p + warp * dsum)*freq, seed + i);
         sum += amp * (1 - abs(n.x));
         dsum += amp * n.yz * -n.x;
         freq *= lacunarity;
         amp *= gain * saturate(sum);
    }
    return sum;
}

float jordanTurbulence(vec2 p, float seed, int octaves)
{
    float lacunarity = 2.0;
    float gain1 = 0.8; float gain = 0.5;
    float warp0 = 0.4; float warp = 0.35;
    float damp0 = 1.0; float damp = 0.8;
    float damp_scale = 1.0;
    vec3 n = perlinNoiseDeriv(p, seed);
    vec3 n2 = n * n.x;
    float sum = n2.x;
    vec2 dsum_warp = warp0*n2.yz;
    vec2 dsum_damp = damp0*n2.yz;

    float amp = gain1;
    float freq = lacunarity;
    float damped_amp = amp * gain;

    for(int i=1; i < octaves; i++)
    {
        n = perlinNoiseDeriv(p * freq + dsum_warp.xy, seed + i / 256.0);
        n2 = n * n.x;
        sum += damped_amp * n2.x;
        dsum_warp += warp * n2.yz;
        dsum_damp += damp * n2.yz;
        freq *= lacunarity;
        amp *= gain;
        damped_amp = amp * (1-damp_scale/(1+dot(dsum_damp,dsum_damp)));
    }
    return sum;
}

float turbulence(vec2 p, float seed, int octaves)
{
    float sum = 0;
    float freq = 1.0, amp = 1.0;
    float lacunarity = 2.0; float gain = 0.5;
    for (int i=0; i < octaves; i++)
    {
        float n = perlinNoise(p*freq, seed + i / 256.0);
        sum += n*amp;
        freq *= lacunarity;
        amp *= gain;
    }
    return sum;
}

float billowedNoise(vec2 p, float seed)
{
    return abs(perlinNoise(p, seed));
}

float ridgedNoise(vec2 p, float seed)
{
    return 1.0f-abs(perlinNoise(p, seed));
}

float iqTurbulence(vec2 p, float seed, int octaves)
{
    float sum = 0.5;
    float freq = 1.0, amp = 1.0;
    float lacunarity = 2.0; float gain = 0.5;
    vec2 dsum = vec2(0,0);
    for (int i=0; i < octaves; i++)
    {
        vec3 n = perlinNoisePseudoDeriv(p*freq, seed + i / 256.0);
        dsum += n.yz;
        sum += amp * n.x / (1 + dot(dsum, dsum));
        freq *= lacunarity;
        amp *= gain;
    }
    return sum;
}
