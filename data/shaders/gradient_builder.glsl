/*******************************************************************************
        Copyright (C) 2014 Andrey Samsonov

        This software is distributed freely under the terms of the MIT License.
        See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
uniform sampler2D height_map;
uniform float param0;
uniform float param1;
#define Dx 2.0/param0
#define Dy 2.0/param1

#ifdef _VERTEX_

in vec3 position;
in vec2 texcoord;

out Vertex {
    vec3  position;
    vec2  uv;
} Vert;

void main(void)
{
    vec4 pos      = vec4(position, 1.0);
    Vert.position = position;
    Vert.uv       = texcoord;

    gl_Position   = pos;
}
#endif

#ifdef _FRAGMENT_

in Vertex {
    vec3  position;
    vec2  uv;
} Vert;

out vec4 color;

void main(void)
{
    const int NUM = 9;

    vec2 c[NUM] = vec2[](
      vec2(-Dx,  Dy),   vec2( 0.00,  Dy),
      vec2( Dx,  Dy),   vec2(-Dx,    0.00 ),
      vec2( 0.0, 0.0 ), vec2( Dx,    Dy ),
      vec2(-Dx, -Dy),   vec2( 0.00, -Dy),
      vec2( Dx, -Dy)
    );

    vec3 col[NUM];
    int i;

    for (i=0; i < NUM; i++) {
      col[i] = texture2D(height_map, Vert.uv + c[i]).xyz;
    }

    highp float x = col[2] +   col[8] + 2*col[5] - col[0] - 2*col[3] - col[6];
    highp float y = col[6] + 2*col[7] +   col[8] - col[0] - 2*col[1] - col[2];

    color = vec4(normalize(vec3(x, y, 0)), 1.0);
}
#endif


