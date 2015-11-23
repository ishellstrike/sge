/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D outline_map;

#ifdef _FRAGMENT_

in vec4 col;
in vec3 pos;
in vec2 uv;

layout (location = 0) out vec4 WorldPosOut;
layout (location = 1) out vec4 DiffuseOut;
layout (location = 2) out vec4 NormalOut;
layout (location = 3) out vec4 TexCoordOut;

void main(void)
{
    vec4 c = texture(diffuse_map, uv);
    if(c.a <= 0.1) discard;
    DiffuseOut = c * col;
    NormalOut = texture(normal_map, uv);
    TexCoordOut = vec4(uv,0,1);
    WorldPosOut = vec4(pos, 1);
}

#endif

#ifdef _VERTEX_

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 color;

uniform mat4 transform_VP;

out vec4 col;
out vec3 pos;
out vec2 uv;


void main(void)
{
    gl_Position = transform_VP * vec4(position, 1);
    col = color;
    pos = position;
    uv = texcoord;
}

#endif

