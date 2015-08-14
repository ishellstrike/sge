/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
uniform sampler2D rgb_map;

#ifdef _VERTEX_

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

out Vertex {
    vec3  position;
    vec2  uv;
} Vert;

void main(void)
{
    Vert.uv       = texcoord;
    gl_Position   = vec4(position, 1.0);
}
#endif

#ifdef _FRAGMENT_

in Vertex {
    vec2  uv;
} Vert;

layout (location = 0) out vec4 color;

void main(void)
{
    vec4 rgb = texture(rgb_map, Vert.uv);
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    float lum = dot(rgb.xyz, W);
    color = vec4(lum, lum, lum, rgb.a);
}
#endif
