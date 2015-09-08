/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#ifdef _FRAGMENT_
#include "turbulence.lib.glsl"

in vec2 fragTexcoord;
in vec4 col;
in vec3 norm;
in vec3 pos;

uniform float iGlobalTime;

layout (location = 0) out vec4 WorldPosOut;
layout (location = 1) out vec4 DiffuseOut;
layout (location = 2) out vec4 NormalOut;
layout (location = 3) out vec4 TexCoordOut;

void main(void)
{
    float f = (perlinNoise(vec2(fragTexcoord.x*100, 0), 1)+1)/2.0;
    vec4 col = vec4(0.8, 0.8, 0.2, 1);
    vec4 col2 = vec4(0.8, 0.4, 0.2, 1);
    col = mix(col, col2, f);
    col.a = (perlinNoise(vec2(fragTexcoord.x*200, 0), 1)+1)/2.0 * 4;
    if(col.a < 0.8) discard;
    col.a = 1;

    DiffuseOut = col;
    TexCoordOut = vec4(fragTexcoord, 0, 1);
    NormalOut = vec4(norm, 1);
    WorldPosOut = vec4(pos, 1);
}

#endif

#ifdef _VERTEX_


layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 color;

uniform mat4 transform_M;
uniform mat4 transform_VP;
uniform mat3 transform_N;

out vec2 fragTexcoord;
out vec4 col;
out vec3 norm;
out vec3 pos;

void main(void)
{
    vec4 pos4 = transform_M * vec4(position, 1);
    pos = pos4.xyz;
    gl_Position = transform_VP * pos4;
    fragTexcoord = texcoord;
    col = color;
    norm = transform_N * normal;
}

#endif

