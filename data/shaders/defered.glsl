/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#include "float.lib.glsl"

uniform sampler2D buffer_world_pos;
uniform sampler2D buffer_diffuse;
uniform sampler2D buffer_normal;
uniform sampler2D buffer_tex_coord;

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix
uniform mat3 transform_N; // normal matrix
uniform vec3 transform_viewPos;
uniform vec3 transform_lightPos;

uniform float R = 1000;
uniform float s = 100;
uniform float screenDistance = 100;

#ifdef _VERTEX_
layout (location = 0) in vec3 position;

out vec2 texcoord;

void main(void)
{
    gl_Position = vec4(position, 1);
    texcoord = position.xy * 0.5 + 0.5;
}
#endif

#ifdef _FRAGMENT_

layout (location = 0) out vec4 color;

in vec2 texcoord;

void main(void)
{
    vec4 dif = texture(buffer_diffuse, texcoord);
    vec3 eye = texture(buffer_normal, texcoord).xyz;
    vec3 pos = texture(buffer_world_pos, texcoord).xyz;
    vec3 light = normalize(transform_lightPos);
    vec3 view_dir = transform_viewPos - pos;

    if(length(dif) <= 1.00) discard;

    const float  k = 1.0;
    float d1 = pow ( max ( dot ( eye, light ), 0.0 ), 1.0 + k );
    float d2 = pow ( 1.0 - dot ( eye, view_dir ), 1.0 - k );

    color = vec4(0,0,0,1);
    color += dif * d1 * d2;
}
#endif
