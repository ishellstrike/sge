/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
uniform sampler2D source_map;

uniform float param0; //off_x
uniform float param1; //off_y
uniform float param2; //size_x
uniform float param3; //size_y

#ifdef _VERTEX_

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

out vec2  uv;

void main(void)
{
    uv   = texcoord;
    gl_Position   = vec4(position, 1.0);
}
#endif

#ifdef _FRAGMENT_

in vec2  uv;

layout (location = 0) out vec4 color;

void main(void)
{
    vec4 rgb = texture(source_map, uv * vec2(param2, param3) + vec2(param0, param1));

    color = rgb;
}
#endif