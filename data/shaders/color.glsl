/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifdef _FRAGMENT_

in vec4 fragColor;
in vec2 uv;

out vec4 out_color;

void main(void)
{
    out_color = fragColor;
}

#endif

#ifdef _VERTEX_

in vec3 position;
in vec2 texcoord;
in vec3 normal;
in vec4 color;

uniform mat4 MVP;

out vec4 fragColor;
out vec2 uv;


void main(void)
{
    gl_Position = MVP * vec4(position, 1);
    fragColor = color;
    uv = texcoord;
}

#endif

