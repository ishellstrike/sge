//autoversion here
/*******************************************************************************
        Copyright (C) 2014 Samsonov Andrey

        This software is distributed freely under the terms of the MIT License.
        See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "noise.lib.glsl"
#include "test1.glsl"
#include "height.lib.glsl"

#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL 2
#define VERT_COLOR 3
#define FRAG_OUTPUT0 0

uniform sampler2D material_texture;
uniform sampler2D material_normal;
uniform vec4  material_ambient;
uniform vec4  material_diffuse;
uniform vec4  material_specular;
uniform vec4  material_emission;
uniform float material_shininess;
uniform float time;

float R = 1000;
float s = 5;

#ifdef _VERTEX_
in vec3 position;
in vec4 color;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;
in vec2 texcoord;

out vec2 texcoordout;
out vec3 normalout;
out vec3 lightVec;
out vec3 viewDir;
out vec3 positionout;
out vec3 plane;
out float deff;
out vec3 eyeNormal;

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix
uniform mat4 transform_N; // normal matrix
uniform vec3 transform_viewPos;
uniform vec3 transform_lightPos;

void main(void)
{
    vec2 vUv = texcoord;

    vec3 grad;
    vec3 grad2;
    vec3 grad3;

    float snoize = (snoise( 5 * position + vec3(0.1,0,0)*time, grad )*5 + snoise( 10 * position  + vec3(-0.1,-0.2,0)*time, grad2 ))/6.0;
    float snoize_terr = ground(position, grad3);
    deff = abs(snoize_terr - snoize);
    grad = (grad*5+grad2)/6.0;
    vec3 newPosition = (R + s * snoize) * position;
    vec4 vertexPosition = transform_M * vec4(newPosition, 1);

    grad = grad / (R + s * snoize);
    plane = grad - (grad * position) * position;

    vec4 lightVec4 = transform_M * vec4(transform_lightPos, 1);
    lightVec = lightVec4.xyz;
    lightVec  = normalize(transform_lightPos);

    gl_Position = transform_VP * vertexPosition;
    positionout = position;
    texcoordout = texcoord;
    normalout = positionout - s * plane;
    viewDir = transform_viewPos - vertexPosition.xyz;
    eyeNormal = vec3(transform_N * vec4(normalout, 0.0));
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;
in vec2 texcoordout;
in vec3 lightVec;
in vec3 normalout;
in vec3 positionout;
in vec3 plane;
in float deff;
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
    vec3 light = normalize(lightVec);
    vec3 view = normalize(viewDir);
    vec3 halfWay = normalize(light + view);
    vec3 eye = normalize(eyeNormal);

    vec4 normalmap = texture2D(material_normal, texcoordout * 100);
    eye = normalize(normalmap.xyz + eye);
    vec4 tex_col = texture2D(material_texture, texcoordout);

    vec4 col2 = texture2D(material_texture, texcoordout*R/10);
    tex_col = (tex_col + col2)/2.0;
    vec4 color = vec4(0,0,0,1);

    float NdotL = max(dot(eye, light), 0.0);
    color += vec4(0.5, 0.5, 1, 1) * NdotL;
    float RdotVpow = max(0.0,pow(dot(eye, halfWay), 180));

    //Fresnel approximation
    float base = max(0, 1-dot(view, light));
    float exp = pow(base, 5);
    float fresnel = fZero + (1-fZero)*exp;

    out_color = color * tex_col;
    out_color.a = 0.1f + deff;
    out_color += vec4(1,1,1,1) * RdotVpow;
    //out_color = normalmap;
}
#endif
