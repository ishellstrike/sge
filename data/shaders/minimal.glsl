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
uniform sampler2D material_height;
uniform sampler2D material_grad;
uniform vec4  material_ambient;
uniform vec4  material_diffuse;
uniform vec4  material_specular;
uniform vec4  material_emission;
uniform float material_shininess;

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix
uniform mat4 transform_N; // normal matrix
uniform vec3 transform_viewPos;
uniform vec3 transform_lightPos;

float R = 1000;
float s = 100;

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
out vec3 eyeNormal;

void main(void)
{
    vec2 vUv = texcoord;

    float snoize = texture2D(material_height, vUv).x;
    vec3 grad = texture2D(material_grad, vUv).xyz;

    vec3 newPosition = (R + s * snoize) * position;
    vec4 vertexPosition = transform_VP * transform_M * vec4(position.xyz*R, 1);

    vec4 lightVec4 = transform_M * vec4(transform_lightPos, 1);
    lightVec = normalize(lightVec4.xyz);

    gl_Position = vertexPosition;
    positionout = position;
    texcoordout = vUv;

    grad = grad / (R + s * snoize);
    vec3 plane = grad - (grad * position) * position;
    normalout = position - s * plane;

    eyeNormal = vec3(transform_N * vec4(normalout, 0.0));
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;
in vec2 texcoordout;
in vec3 lightVec;
in vec3 normalout;
in vec3 positionout;
in vec3 eyeNormal;

const vec4 fog = vec4(100/255.f, 149/255.f, 237/255.f, 1.f);
float density = 0.0003;
const float LOG2 = 1.442695;
const vec4 colc = vec4(0.5, 0.5, 0.5, 1);

out vec4 out_color;

void main(void)
{
    vec3 grad = texture2D(material_grad, texcoordout).xyz*50;
    float snoize = texture2D(material_height, texcoordout).x;
    grad = grad / (R + s * snoize);
    vec3 plane = grad - (grad * positionout) * positionout;
    vec3 normal = positionout - s * plane;
    vec3 eye = normalize(vec3(transform_N * vec4(normal, 0.0)));
    vec3 light = normalize(lightVec);

    vec4 tex = texture2D(material_texture, texcoordout);
    vec4 col = material_ambient;

    float NdotL = clamp(dot(light, eye), 0, 1);

    col += material_diffuse * NdotL;

    //float z = gl_FragCoord.z / gl_FragCoord.w;
    //  float fogFactor = exp2( -density * density * z * z * LOG2 );
    // fogFactor = clamp(fogFactor, 0.0, 1.0);
    //col = mix(fog, col, fogFactor);
    col.a = 1;
    out_color = col * tex;
}
#endif
