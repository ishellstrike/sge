/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#include "turbulence.lib.glsl"
#include "float.lib.glsl"

uniform sampler2D inputTex0; //random texture
uniform float param0; //off_x
uniform float param1; //off_y
uniform float param2; //scale

#ifdef _VERTEX_

in vec3 position;
in vec2 texcoord;

out Vertex {
    vec3 position;
    vec2 texcoord;
} Vert;

void main(void)
{
    vec4 pos      = vec4(position, 1.0);
    Vert.position = position;
    Vert.texcoord = vec2(param0, param1)+texcoord*param2;

    gl_Position   = pos;
}
#endif

#ifdef _FRAGMENT_

in Vertex {
    vec3  position;
    vec2 texcoord;
} Vert;

out vec4 color;

#define PI 3.14159265358979

float hash( in vec3 p )
{
    return fract(sin(p.x*15.32758341+p.y*39.786792357+p.z*59.4583127+7.5312) * 43758.236237153)-.5;
}

vec3 hash3( in vec3 p )
{
    return vec3(hash(p),hash(p+1.5),hash(p+2.5));
}

//concentric waves variant
float gavoronoi3b(in vec3 p)
{
    vec3 ip = floor(p);
    vec3 fp = fract(p);
    float f = 2.*PI;																																										;//frequency
    float v = .8;//cell variability <1.
    float va = 0.0;
    float wt = 0.0;
    for (int i=-1; i<=1; i++)
    for (int j=-1; j<=1; j++)
    for (int k=-1; k<=1; k++)
    {
        vec3 o = vec3(i, j, k)-.5;
        vec3 pp = fp +o  - v*hash3(ip - o);
        float d = length(pp);
        float w = exp(-d*4.);
        wt +=w;
        va +=sin(sqrt(d)*f)*w;
    }
    return va/wt;
}



float fbmabs( vec3 p ) {

    float f=1.2;

    float r = 0.0;
    for(int i = 0;i<5;i++){
        r += abs(gavoronoi3b( p*f ))/f;
        f *=2.3;
    }
    return r/2.;
}

float map(vec3 p)
{
    return 1.2*fbmabs(p);
}

void main(void)
{
    float c =  map(vec3(Vert.texcoord*100, 1));
    float pp = c;//jordanTurbulence(Vert.texcoord*100, 0, 18);

    pp = clamp(pp, 0.0001, 0.9999);
    pp = pp;
    color = encodeFloat(pp);
    //color.w = 1;
}
#endif
