/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

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
    float v = .98;//cell variability <1.
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
        f *= 15.3;
    }
    return r/2.0;
}

float crater(vec3 p, float a)
{
    return clamp(0.4*fbmabs(p), 0.0001, 0.9999) ;
}
