/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
uniform sampler2D rgb_map;
uniform sampler2D rgb_map2;
uniform sampler2D lerp_map;

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
    vec4 rgb = texture(rgb_map, uv);
    vec4 rgb2 = texture(rgb_map2, uv);
    float factor = texture(lerp_map, uv).r;

    color = mix(rgb, rgb2, factor);
}
#endif
