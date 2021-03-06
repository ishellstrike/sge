/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifdef _FRAGMENT_

uniform sampler2D material_texture;
in vec2 fragTexcoord;
in vec4 col;

out vec4 out_color;

void main(void)
{
    out_color = col * vec4(1, 1, 1, texture(material_texture, fragTexcoord).a);

    if(out_color.a < 0.1)
        discard;
}

#endif

#ifdef _VERTEX_

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 color;

uniform mat4 transform_VP;

out vec2 fragTexcoord;
out vec4 col;

void main(void)
{
    gl_Position = transform_VP * vec4(position, 1);
    fragTexcoord = texcoord;
    col = color;
}

#endif

