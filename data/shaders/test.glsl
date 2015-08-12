/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT License.
        See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL 2
#define VERT_COLOR 3
#define FRAG_OUTPUT0 0

#include "float.lib.glsl"

uniform sampler2D material_texture;

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix

#ifdef _VERTEX_
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texcoord;


out VS_OUT {
    vec2 texcoordout;
} vert;

void main(void)
{
    gl_Position = transform_VP * transform_M * vec4(position, 1);
    vert.texcoordout = texcoord;
}
#endif

#ifdef _FRAGMENT_

in VS_OUT {
    vec2 texcoordout;
} frag_in;

layout(location = 0) out vec4 out_color;

void main(void)
{
    vec2 texcoordout = frag_in.texcoordout;

    vec4 tex = texture2D(material_texture, texcoordout);

    out_color = tex;
}
#endif
