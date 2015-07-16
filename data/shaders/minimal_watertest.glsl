//autoversion here
/*******************************************************************************
        Copyright (C) 2014 Samsonov Andrey

        This software is distributed freely under the terms of the MIT License.
        See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "noise.lib.glsl"
#include "test1.glsl"

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

float R = 1010;
float s = 20;

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
out vec3 positionout;
out vec3 plane;
out float deff;

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix
uniform mat3 transform_N; // normal matrix
uniform vec3 transform_viewPos;
uniform vec3 transform_lightPos;

void main(void)
{
    vec2 vUv = texcoord;

    vec3 grad;
    vec3 grad2;
    vec3 grad3;

    float snoize = (snoise( 5 * position + vec3(0.1,0,0)*time, grad )*5 + snoise( 100 * position, grad2 ))/6.0;
    float snoize_terr = (snoise( 5 * position, grad3 )*5 + snoise( 100 * position, grad3 ))/6.0;
    deff = abs(snoize_terr - snoize);
    grad = (grad*5+grad2)/6.0;
    vec3 newPosition = (R + s * snoize) * position;
    vec4 vertexPosition = transform_VP * transform_M * vec4(newPosition, 1);

    grad = grad / (R + s * snoize);
    plane = grad - (grad * position) * position;

    vec4 lightVec4 = transform_M * vec4(transform_lightPos, 1);
    lightVec = normalize(lightVec4.xyz);

    gl_Position = vertexPosition;
    positionout = position;
    texcoordout = texcoord;
    normalout = positionout - s * plane;
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
const vec4 fog = vec4(100/255.f, 149/255.f, 237/255.f, 1.f);
float density = 0.0003;
const float LOG2 = 1.442695;
const vec4 colc = vec4(0.5, 0.5, 0.5, 1);

out vec4 out_color;

void main(void)
{
    vec3 normal = -normalout;
    float DiffuseFactor = dot(normalize(normal), -lightVec);
    vec4 col = texture2D(material_texture, texcoordout) * DiffuseFactor;
    vec4 col2 = texture2D(material_texture, texcoordout*R/10) * DiffuseFactor;
    col = (col + col2)/2.0;
    if (DiffuseFactor <= 0) {
        col =  vec4(0,0,0,1);
    }
    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = exp2( -density * density * z * z * LOG2 );
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    col = mix(fog, col, fogFactor);
    col.a = deff;
    out_color = col;
}
#endif