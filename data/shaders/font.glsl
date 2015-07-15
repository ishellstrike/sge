//autoversion here
/*******************************************************************************
        Copyright (C) 2014 Samsonov Andrey

        This software is distributed freely under the terms of the MIT License.
        See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

#ifdef _FRAGMENT_

uniform sampler2D colorTexture;
in vec2 fragTexcoord;
in vec4 col;

out vec4 out_color;

void main(void)
{
    out_color = col * vec4(1, 1, 1, texture2D(colorTexture, fragTexcoord).a);
}

#endif

#ifdef _VERTEX_

in vec3 position;
in vec2 texcoord;
in vec3 normal;
in vec4 color;

uniform mat4 MVP;

out vec2 fragTexcoord;
out vec4 col;

void main(void)
{
    gl_Position = MVP * vec4(position, 1);
    fragTexcoord = texcoord;
    col = color;
}

#endif

