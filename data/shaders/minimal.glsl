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

uniform struct Material
{
        sampler2D texture;
        sampler2D normal;
        vec4  ambient;
        vec4  diffuse;
        vec4  specular;
        vec4  emission;
        float shininess;
} material;

#ifdef _VERTEX_
attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 normal;
attribute vec4 color;
varying vec2 texcoordout;
varying vec3 viewDir;
varying vec4 normalout;

uniform struct Transform
{
        mat4 model;
        mat4 viewProjection;
        mat3 normal;
        vec3 viewPosition;
} transform;

void main(void)
{
    vec4 vertex = transform.model * vec4(position, 1.0);
    viewDir  = normalize(transform.viewPosition - vertex.xyz);
    gl_Position = transform.viewProjection * vertex;
    texcoordout = texcoord;
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;
varying vec2 texcoordout;
varying vec3 viewDir;
varying vec4 normalout;

void main(void)
{
    vec3 a = texture2D(material.texture, texcoordout).xyz * dot(texture2D(material.normal, texcoordout).xyz, -viewDir);
    gl_FragColor = vec4(a, 1);
}
#endif
