/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifdef _FRAGMENT_

in vec4 fragColor;
in vec3 pos;

layout (location = 0) out vec4 WorldPosOut;
layout (location = 1) out vec4 DiffuseOut;
layout (location = 2) out vec4 NormalOut;
layout (location = 3) out vec4 TexCoordOut;

void main(void)
{
    DiffuseOut = fragColor;
    NormalOut = vec4(1,0,0,1);
    TexCoordOut = vec4(1,0,0,1);
    WorldPosOut = vec4(pos, 1);
}

#endif

#ifdef _VERTEX_

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 color;

uniform mat4 transform_VP;

out vec4 fragColor;
out vec3 pos;


void main(void)
{
    gl_Position = transform_VP * vec4(position, 1);
    fragColor = color;
    pos = position;
}

#endif

