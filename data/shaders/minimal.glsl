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
    vec3 grad = decodeNormal(texture2D(material_grad, texcoord))*150;

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
    float snoize = decodeFloat(textureLod(material_height, texcoordout, 0));
    vec3 grad = decodeNormal(texture2D(material_grad, texcoordout))*10;

    grad = grad / (R + s * snoize);
    grad = transform_N * grad;
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
    if(snoize > 0.6)
    {
        tex = texture2D(material_high, texcoordout2*10);
        tex += texture2D(material_high, texcoordout2*100);
    }

    if(abs(grad.x) + abs(grad.y) > 30)
    {
        vec4 tex3 = texture2D(material_side, texcoordout2*10);
        tex3 += texture2D(material_side, texcoordout2*100);

        tex = mix(tex, tex3, clamp((abs(grad.x) + abs(grad.y)-30), 0 ,1));
    }

    vec4 col = material_ambient;

    //tex = mix(vec4(0,1,0,1), vec4(1,0,0,1), abs(grad.y) + abs(grad.x));

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
