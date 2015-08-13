/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#include "noise.lib.glsl"
#include "test1.glsl"
#include "height.lib.glsl"
#include "float.lib.glsl"

#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL 2
#define VERT_COLOR 3
#define FRAG_OUTPUT0 0

uniform sampler2D material_texture;
uniform sampler2D material_normal;
uniform sampler2D material_height;
uniform sampler2D material_grad;
uniform sampler2D material_global_height;
uniform vec4  material_ambient;
uniform vec4  material_diffuse;
uniform vec4  material_specular;
uniform vec4  material_emission;
uniform float material_shininess;

uniform float time;

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix
uniform mat3 transform_N; // normal matrix
uniform vec3 transform_viewPos;
uniform vec3 transform_lightPos;

uniform float R = 1010;
uniform float s = 1;

vec3 offsetedPosition(vec3 pos, float time)
{
    return pos*100 + pos*1000 + pos*10 + vec3(1,1,1)*time - vec3(0.8,0.2,0.4)*time  + vec3(0.2,0.1,0.2)*time;
}

#ifdef _VERTEX_
in vec3 position;
in vec4 color;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;
in vec2 texcoord;
in vec2 texcoord2;

out vec2 texcoordout;
out vec2 texcoordout2;
out vec3 normalout;
out vec3 lightVec;
out vec3 viewDir;
out vec3 positionout;
out vec3 plane;
out vec3 eyeNormal;

void main(void)
{
    vec3 grad;
    float snoize = snoise(offsetedPosition(position, time), grad);

    vec3 newPosition = (R + s * snoize) * position;
    vec4 mLocation = transform_M * vec4(newPosition, 1);
    vec4 mvpLocation = transform_VP * mLocation;

    lightVec = transform_lightPos;

    gl_Position = mvpLocation;
    positionout = position;
    texcoordout = texcoord;
    texcoordout2 = texcoord2;
    viewDir = transform_viewPos - vec3(mLocation);

    grad = grad / (R + s * snoize);
    vec3 plane = grad - (grad * position) * position;
    normalout = position - s * plane;

    eyeNormal = transform_N * normalout;
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;
in vec2 texcoordout;
in vec2 texcoordout2;
in vec3 lightVec;
in vec3 normalout;
in vec3 positionout;
in vec3 plane;
in vec3 viewDir;
in vec3 eyeNormal;

const vec4 fog = vec4(100/255.f, 149/255.f, 237/255.f, 1.f);
float density = 0.0003;
const float LOG2 = 1.442695;
const vec4 colc = vec4(0.5, 0.5, 0.5, 1);

const float fZero = 0.01800940761;// pow((1.0f-(1.0f/1.31f))/(1.0f+(1.0f/1.31f)), 2);

out vec4 out_color;

void main(void)
{
    vec3 grad;
    float snoize = snoise(offsetedPosition(positionout, time), grad);
    float snoize_terr = decodeFloat(textureLod(material_global_height, texcoordout2, 0));
    //vec3 grad = decodeNormal(texture2D(material_grad, texcoordout))*100;
    float deff = abs((snoize_terr*100 + 1000) - (snoize*s + 1010))/100;

    grad = grad / (R + s * snoize);
    grad = transform_N * grad * 50;
    vec3 plane = grad - (grad * positionout) * positionout;
    vec3 normal = positionout - s * plane;
    vec3 eye = normalize(transform_N * normal);
    vec3 light = normalize(lightVec);
    vec3 view = normalize(viewDir);
    vec3 halfWay = normalize(light + view);

    vec4 tex_col = texture2D(material_texture, texcoordout2*100);
    vec4 col2 = texture2D(material_texture, texcoordout2*R/10);

    tex_col = (tex_col + col2)/2.0;
    vec4 color = material_ambient;

    float NdotL = clamp(dot(light, eye), 0, 1);
    color += material_diffuse * NdotL;
    float RdotVpow = max(0.0,pow(dot(eye, halfWay), material_shininess));

    //Fresnel approximation
    float base = max(0, 1-dot(view, light));
    float exp = pow(base, 5);
    float fresnel = fZero + (1-fZero)*exp;

    out_color = color;
    out_color.a = deff * 10;
    out_color += material_specular * fresnel * RdotVpow;
    //out_color = textureLod(material_height, texcoordout, 0);
}
#endif
