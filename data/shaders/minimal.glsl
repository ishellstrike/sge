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

#ifdef _VERTEX_
in vec3 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;
in vec2 texcoord2;

out vec2 texcoordout;
out vec2 texcoordout2;
out vec3 lightVec;
out vec3 positionout;

void main(void)
{
    float snoize = decodeFloat(textureLod(material_global_height, texcoord2, 0));

    vec3 newPosition = (R + s * snoize) * position;
    vec4 mvpLocation = transform_VP * transform_M * vec4(newPosition, 1);

    lightVec = transform_lightPos;

    gl_Position = mvpLocation;
    positionout = position;
    texcoordout = texcoord;
    texcoordout2 = texcoord2;
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;
in vec2 texcoordout;
in vec2 texcoordout2;
in vec3 lightVec;
in vec3 positionout;

const vec4 fog = vec4(100/255.f, 149/255.f, 237/255.f, 1.f);
float density = 0.0003;
const float LOG2 = 1.442695;
const vec4 colc = vec4(0.5, 0.5, 0.5, 1);

out vec4 out_color;

void main(void)
{
    float snoize = decodeFloat(texture2D(material_height, texcoordout));
    vec3 grad = decodeNormal(texture2D(material_grad, texcoordout));

    grad = grad / (R + s * snoize);
    //grad = transform_N * grad;
    vec3 plane = grad - (grad * positionout) * positionout;
    vec3 normal = positionout - s * plane;
    vec3 eye = normalize(transform_N * normal);
    vec3 light = normalize(lightVec);

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
    col.a = 1;
    out_color = col * tex + material_emission;
}
#endif

#ifdef _TESSEVAL_
layout(quads, fractional_odd_spacing) in;

in TCS_OUT
{
    vec2 tc;
} tes_in[];

out TES_OUT
{
    vec2 tc;
} tes_out;

void main(void)
{
    mat4 mvp = transform_VP * transform_M;

    //vec2 tc1 = mix(tes_in[0].tc, tes_in[1].tc, gl_TessCoord.x);
    //vec2 tc2 = mix(tes_in[2].tc, tes_in[3].tc, gl_TessCoord.x);
    vec2 tc;//mix(tc2, tc1, gl_TessCoord.y);

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    tc = vec2(u, v);

    vec4 p1 = mix(gl_in[0].gl_Position,
                  gl_in[1].gl_Position,
                  gl_TessCoord.x);
    vec4 p2 = mix(gl_in[2].gl_Position,
                  gl_in[3].gl_Position,
                  gl_TessCoord.x);
    vec4 p = mix(p2, p1, gl_TessCoord.y);

    p.z += texture(material.texture, tc).r * 0.1;
    //p.z += (sin(u*3.1415*2)+cos(v*3.1415*2))/10.0;

    tes_out.tc = tc;
    gl_Position = mvp * p;
}
#endif

#ifdef _TESSCONTROL_
layout ( vertices = 4 ) out;

in VS_OUT
{
    vec2 tc;
} tcs_in[];

out TCS_OUT
{
    vec2 tc;
} tcs_out[];

void main ()
{

        if (gl_InvocationID == 0)
    {
        mat4 mvp = transform_VP * transform_M;
        vec4 p0 = mvp * gl_in[0].gl_Position;
        vec4 p1 = mvp * gl_in[1].gl_Position;
        vec4 p2 = mvp * gl_in[2].gl_Position;
        vec4 p3 = mvp * gl_in[3].gl_Position;
        p0 /= p0.w;
        p1 /= p1.w;
        p2 /= p2.w;
        p3 /= p3.w;
        if (p0.z <= 0.0 ||
            p1.z <= 0.0 ||
            p2.z <= 0.0 ||
            p3.z <= 0.0)
        {
             gl_TessLevelOuter[0] = 0.0;
             gl_TessLevelOuter[1] = 0.0;
             gl_TessLevelOuter[2] = 0.0;
             gl_TessLevelOuter[3] = 0.0;
        }
        else
        {
            float l0 = length(p2.xy - p0.xy) * 32.0 + 16.0;
            float l1 = length(p3.xy - p2.xy) * 32.0 + 16.0;
            float l2 = length(p3.xy - p1.xy) * 32.0 + 16.0;
            float l3 = length(p1.xy - p0.xy) * 32.0 + 16.0;
            gl_TessLevelOuter[0] = l0;
            gl_TessLevelOuter[1] = l1;
            gl_TessLevelOuter[2] = l2;
            gl_TessLevelOuter[3] = l3;
            gl_TessLevelInner[0] = min(l1, l3);
            gl_TessLevelInner[1] = min(l0, l2);
        }
    }
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tcs_out[gl_InvocationID].tc = tcs_in[gl_InvocationID].tc;
}
#endif
