/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#include "float.lib.glsl"
uniform sampler2D height_map;
uniform float param0;
uniform float param1;
uniform float param2;
#define Dx 1.0/param0
#define Dy 1.0/param1

#ifdef _VERTEX_

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

out Vertex {
    vec3  position;
    vec2  uv;
} Vert;

void main(void)
{
    vec4 pos      = vec4(position, 1.0);
    Vert.position = position;
    Vert.uv       = texcoord;

    gl_Position   = pos;
}
#endif

#ifdef _FRAGMENT_

in Vertex {
    vec3  position;
    vec2  uv;
} Vert;

out vec4 color;

float ApplyChainRule(float dhdu, float dhdv, float dud_, float dvd_)
{
    return dhdu * dud_ + dhdv * dvd_;
}

vec3 CalculateSurfaceGradient(vec3 n, vec3 dpdx, vec3 dpdy, float dhdx, float dhdy)
{
    vec3 r1 = cross(dpdy, n);
    vec3 r2 = cross(n, dpdx);

    return (r1 * dhdx + r2 * dhdy) / dot(dpdx, r1);
}

vec3 PerturbNormal(vec3 normal, vec3 dpdx, vec3 dpdy, float dhdx, float dhdy)
{
    return normalize(normal - CalculateSurfaceGradient(normal, dpdx, dpdy, dhdx, dhdy));
}

vec3 CalculateSurfaceNormal(vec3 position, vec3 normal, float height)
{
    vec3 dpdx = dFdx(position);
    vec3 dpdy = dFdy(position);

    float dhdx = dFdx(height);
    float dhdy = dFdy(height);

    return PerturbNormal(normal, dpdx, dpdy, dhdx, dhdy);
}

void main(void)
{
    const int NUM = 9;

    vec2 c[NUM] = vec2[](
      vec2(-Dx,  Dy),   vec2( 0.00,  Dy),
      vec2( Dx,  Dy),   vec2(-Dx,    0.00 ),
      vec2( 0.0, 0.0 ), vec2( Dx,    Dy ),
      vec2(-Dx, -Dy),   vec2( 0.00, -Dy),
      vec2( Dx, -Dy)
    );

    float col[NUM];
    int i;

    for (i=0; i < NUM; i++) {
      vec4 tex = texture(height_map, Vert.uv + c[i]);
      col[i] = decodeFloat(tex);
    }

    //[-1, 1]
    float x = (col[2] +   col[8] + 2*col[5] - col[0] - 2*col[3] - col[6])/4.0;
    float y = (col[6] + 2*col[7] +   col[8] - col[0] - 2*col[1] - col[2])/4.0;
    vec3 v3 = vec3(x / param2, y / param2, 0);
    v3 = clamp(v3, -0.99, 0.99);
    //v3 = normalize(v3);

    color = encodeNormal(v3);
}
#endif


