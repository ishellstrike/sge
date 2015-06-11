/*******************************************************************************
        Copyright (C) 2014 Samsonov Andrey

        This software is distributed freely under the terms of the MIT License.
        See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

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

#ifdef _VERTEX_
attribute vec3 position;
attribute vec4 color;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 binormal;
attribute vec2 texcoord;

varying vec2 texcoordout;
varying vec3 normalout;
varying vec3 lightVec;

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix
uniform mat3 transform_N; // normal matrix
uniform vec3 transform_viewPos;
uniform vec3 transform_lightPos;

void main(void)
{
    vec4 vertex = transform_M *  vec4(position, 1);
    vec4 vertexPosition = transform_VP * vertex;

    vec4 lightVec4 = transform_M * vec4(transform_lightPos, 1);
    lightVec = normalize(lightVec4.xyz);

    gl_Position = vertexPosition;
    texcoordout = texcoord;
    vec4 normalout4 = transform_M * vec4(normal, 0);
    normalout = normalout4.xyz;
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;
varying vec2 texcoordout;
varying vec3 lightVec;
varying vec3 normalout;
const vec4 fog = vec4(100/255.f, 149/255.f, 237/255.f, 1.f);
float density = 0.0003;
const float LOG2 = 1.442695;
const vec4 colc = vec4(0.5, 0.5, 0.5, 1);

void main(void)
{
    float DiffuseFactor = dot(normalize(normalout), -lightVec);
    vec4 col;
    if (DiffuseFactor > 0) {
        col =  colc  * DiffuseFactor;
    }
    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = exp2( -density * density * z * z * LOG2 );
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    col = mix(fog, col, fogFactor);
    col.a = 1;
    gl_FragColor = col;
}
#endif
