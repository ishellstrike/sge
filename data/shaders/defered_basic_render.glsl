/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifdef _VERTEX_

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

uniform mat4 transform_VP;

out vec3 pos;
out vec3 norm;
out vec2 uv;

void main(void)
{
    gl_Position = transform_VP * vec4(position, 1);
    pos = position;
    uv = texcoord;
    norm = normal;
}

#endif


#ifdef _FRAGMENT_

in vec3 pos;
in vec3 norm;
in vec2 uv;

layout (location = 0) out vec4 WorldPosOut;
layout (location = 1) out vec4 DiffuseOut;
layout (location = 2) out vec4 NormalOut;
layout (location = 3) out vec4 TexCoordOut;

void main(void)
{
    DiffuseOut = vec4(1, 0, 0, 1);
    NormalOut = vec4(norm, 1);
    TexCoordOut = vec4(uv, 0, 1);
    WorldPosOut = vec4(pos, 1);
}

#endif
