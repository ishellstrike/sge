/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#include "turbulence.lib.glsl"
#include "float.lib.glsl"
#include "crater.lib.glsl"

uniform sampler2D inputTex0; //random texture
uniform float param0; //off_x
uniform float param1; //off_y
uniform float param2; //scale

#ifdef _VERTEX_

in vec3 position;
in vec2 texcoord;

out Vertex {
    vec3 position;
    vec2 texcoord;
} Vert;

void main(void)
{
    vec4 pos      = vec4(position, 1.0);
    Vert.position = position;
    Vert.texcoord = vec2(param0, param1)+texcoord*param2;

    gl_Position   = pos;
}
#endif

#ifdef _FRAGMENT_

in Vertex {
    vec3  position;
    vec2 texcoord;
} Vert;

out vec4 color;

void main(void)
{
    float c =  crater(vec3(Vert.texcoord*100, 1), 10);
    float pp = jordanTurbulence(Vert.texcoord*10, 0, 18) - c;
    pp = clamp(pp, 0, 1);

    color = encodeFloat(pp);
    //color.w = 1;
}
#endif
