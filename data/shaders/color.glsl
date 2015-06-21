/*******************************************************************************
        Copyright (C) 2014 Samsonov Andrey

        This software is distributed freely under the terms of the MIT License.
        See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

#ifdef _FRAGMENT_

varying vec4 fragColor;
varying vec2 uv;

void main(void)
{
    gl_FragColor = fragColor;
}

#endif

#ifdef _VERTEX_

attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 normal;
attribute vec4 color;

uniform mat4 MVP;

varying vec4 fragColor;
varying vec2 uv;


void main(void)
{
    gl_Position = MVP * vec4(position, 1);
    fragColor = color;
    uv = texcoord;
}

#endif

