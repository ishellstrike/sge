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
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix
uniform mat3 transform_N; // normal matrix
uniform vec3 transform_viewPos;
uniform vec3 transform_lightPos;

void main(void)
{
    //t basis
    vec3 n = normalize (transform_N * normal);
    vec3 t = normalize (transform_N * tangent);
    vec3 b = normalize (transform_N * binormal);

    vec4 vertex = transform_M *  vec4(position, 1);
    vec4 vertexPosition = transform_VP * vertex;
    vec3 lightDir = normalize(transform_lightPos - vec3(vertexPosition));

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
    vec3 normal = 2.0 * texture2D (material_normal, texcoordout).rgb - 1.0;
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
            diffuseMaterial = texture2D (material_texture, texcoordout);
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
