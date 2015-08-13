/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix

#ifdef _FRAGMENT_

in vec4 colorout;

out vec4 out_color;

void main(void)
{
    out_color = colorout;
}

#endif

#ifdef _VERTEX_

in vec3 position;
in vec4 color;

out vec4 colorout;

void main(void)
{
    gl_Position = transform_VP * transform_M * vec4(position, 1);
    colorout = color;
}

#endif

