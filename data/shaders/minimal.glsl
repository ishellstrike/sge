/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL 2
#define VERT_COLOR 3
#define FRAG_OUTPUT0 0

#include "float.lib.glsl"

uniform sampler2D material_texture;
uniform sampler2D material_normal;
uniform sampler2D material_height;
uniform sampler2D material_grad;
uniform sampler2D material_global_height;

uniform sampler2D material_detail;
uniform sampler2D material_low;
uniform sampler2D material_medium;
uniform sampler2D material_high;
uniform sampler2D material_side;

uniform vec4  material_ambient;
uniform vec4  material_diffuse;
uniform vec4  material_specular;
uniform vec4  material_emission;
uniform float material_shininess;

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
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texcoord;
layout (location = 4) in vec2 texcoord2;


out VS_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
} vert;

void main(void)
{
    gl_Position = vec4(position, 1);
    vert.texcoordout = texcoord;
    vert.texcoordout2 = texcoord2;
}
#endif

#ifdef _GEOMETRY_
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
} geo_in[3];

out GEO_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} geo_out;

void main(void)
{
    for(int i = 0; i < 3; ++i)
    {
        geo_out.texcoordout = geo_in[i].texcoordout;
        geo_out.texcoordout2 = geo_in[i].texcoordout2;
        float snoize = decodeFloat(texture(material_global_height, geo_out.texcoordout2));
        vec3 pos = gl_in[i].gl_Position.xyz;
        vec3 newPosition = (R + s * snoize) * pos;
        vec4 mvpLocation = transform_VP * transform_M * vec4(newPosition, 1);

        geo_out.positionout = gl_in[i].gl_Position.xyz;
        gl_Position = mvpLocation;

        EmitVertex();
    }

    EndPrimitive();
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;

in GEO_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} frag_in;

const vec4 fog = vec4(100/255.f, 149/255.f, 237/255.f, 1.f);
float density = 0.0003;
const float LOG2 = 1.442695;
const vec4 colc = vec4(0.5, 0.5, 0.5, 1);

layout(location = 0) out vec4 out_color;

void main(void)
{
    vec2 texcoordout = frag_in.texcoordout;
    vec2 texcoordout2 = frag_in.texcoordout2;
    vec3 positionout = frag_in.positionout;

    float snoize = decodeFloat(texture(material_height, texcoordout));
    vec3 grad = decodeNormal(texture(material_grad, texcoordout));

    grad = grad / (R + s * snoize);
    //grad = transform_N * grad;
    vec3 plane = grad - (grad * positionout) * positionout;
    vec3 normal = positionout - s * plane;
    vec3 eye = normalize(transform_N * normal);
    vec3 light = normalize(transform_lightPos);

    vec4 tex;
    vec4 tex2;

    tex = texture2D(material_low, texcoordout2*10);
    tex += texture2D(material_low, texcoordout2*100);
    tex /= 2;
    tex2 = texture2D(material_medium, texcoordout2*10);
    tex2 += texture2D(material_medium, texcoordout2*100);
    tex2 /= 2;
    tex = mix(tex, tex2, snoize+0.4);
    if(snoize > 0.5)
    {
        tex = texture2D(material_high, texcoordout2*10);
        tex += texture2D(material_high, texcoordout2*100);
    }

    vec4 tex3 = texture2D(material_side, texcoordout2*10);
    tex3 += texture2D(material_side, texcoordout2*100);
    tex = mix(tex, tex3, clamp(abs(grad.y) + abs(grad.x) * 5, 0, 1));

    vec4 col = material_ambient;

    float diffuse_rate = clamp(dot(light, eye), 0, 1);

    col += material_diffuse * diffuse_rate;

    //float z = gl_FragCoord.z / gl_FragCoord.w;
    //float fogFactor = exp2( -density * density * z * z * LOG2 );
    //fogFactor = clamp(fogFactor, 0.0, 1.0);
    //col = mix(fog, col, fogFactor);
    out_color = col * tex + material_emission;
    out_color.a = 1;
}
#endif
