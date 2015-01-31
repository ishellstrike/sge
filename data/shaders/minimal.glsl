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
attribute vec4 color;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 binormal;
attribute vec2 texcoord;

varying vec2 texcoordout;
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;

uniform mat4 transformmodel;
uniform mat4 transformviewProjection;
uniform mat3 transformnormal;
uniform vec3 transformviewPosition;
uniform vec3 transformlightPosition;

void main(void)
{
    //t basis
    vec3 n = normalize (transformnormal * normal);
    vec3 t = normalize (transformnormal * tangent);
    vec3 b = normalize (transformnormal * binormal);

    vec4 vertex = transformmodel *  vec4(position, 1);
    vec4 vertexPosition = transformviewProjection * vertex;
    vec3 lightDir = normalize(transformlightPosition - vec3(vertexPosition));

    vec3 v;
    v.x = dot (lightDir, t);
    v.y = dot (lightDir, b);
    v.z = dot (lightDir, n);
    lightVec = normalize (v);

    v.x = dot (vec3(vertexPosition), t);
    v.y = dot (vec3(vertexPosition), b);
    v.z = dot (vec3(vertexPosition), n);
    eyeVec = normalize (v);

    vec3 halfVector = normalize(vec3(vertexPosition) + lightDir);
    v.x = dot (halfVector, t);
    v.y = dot (halfVector, b);
    v.z = dot (halfVector, n);
    halfVec = v ;

    gl_Position = vertexPosition;
    texcoordout = texcoord;
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;
varying vec2 texcoordout;
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;

void main(void)
{
    // lookup normal from normal map, move from [0,1] to  [-1, 1] range, normalize
    vec3 normal = 2.0 * texture2D (material.normal, texcoordout).rgb - 1.0;
    normal = normalize (normal);

    // compute diffuse lighting
    float lamberFactor= max (dot (lightVec, normal), 0.0) ;
    vec4 diffuseMaterial;
    vec4 diffuseLight;

    // compute specular lighting
    vec4 specularMaterial ;
    vec4 specularLight ;
    float shininess;

    // compute ambient
    vec4 ambientLight = vec4(0.1,0.1,0.1,1);

    if (lamberFactor > 0.0)
    {
            diffuseMaterial = texture2D (material.texture, texcoordout);
            diffuseLight  = vec4(0.4,0.4,0.4,1);

            // In doom3, specular value comes from a texture
            specularMaterial =  vec4(1.0)  ;
            specularLight = vec4(1,1,1,1);
            shininess = pow (max (dot (halfVec, normal), 0.0), 2.0);

            gl_FragColor =	diffuseMaterial * diffuseLight * lamberFactor;
            gl_FragColor +=	specularMaterial * specularLight * shininess;

    }

    gl_FragColor +=	ambientLight;
}
#endif
