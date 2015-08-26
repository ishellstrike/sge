/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#include "float.lib.glsl"
#include "turbulence.lib.glsl"

uniform mat4 transform_VP; // view * projection matrix

uniform float iGlobalTime;

#ifdef _VERTEX_
layout (location = 0) in vec3 position;

out vec2 fragCoord;

void main(void)
{
    gl_Position = vec4(position, 1);
    fragCoord = position.xy * 0.5 + 0.5;
}
#endif

#ifdef _FRAGMENT_

layout (location = 0) out vec4 fragColor;

in vec2 fragCoord;

// original by nimitz https://www.shadertoy.com/view/lsSGzy#, slightly modified

#define ray_brightness 10.
#define gamma 5.
#define ray_density 4.5
#define curvature 15.
#define red   4.
#define green 1.0
#define blue  .3

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!! UNCOMMENT ONE OF THESE TO CHANGE EFFECTS !!!!!!!!!!!
// MODE IS THE PRIMARY MODE
#define MODE normalize
// #define MODE

#define MODE3 *
// #define MODE3 +

#define MODE2 r +
// #define MODE2

// #define DIRECTION +
#define DIRECTION -

#define SIZE 0.1

#define INVERT /
// #define INVERT *
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// FLARING GENERATOR, A.K.A PURE AWESOME
mat2 m2 = mat2( 0.80,  0.60, -0.60,  0.80 );
float fbm( in vec2 p )
{
        float z=2.;       // EDIT THIS TO MODIFY THE INTENSITY OF RAYS
        float rz = 0.00015; // EDIT THIS TO MODIFY THE LENGTH OF RAYS
        p *= 01.25;       // EDIT THIS TO MODIFY THE FREQUENCY OF RAYS
        for (int i= 1; i < 6; i++)
        {
                rz+= abs((perlinNoise(p, 1)-0.5)*2.)/z;
                z = z*2.;
                p = p*2.*m2;
        }
        return rz;
}

const vec2 iResolution = vec2(1,1);
const vec2 iMouse = vec2(0);

void main( void )
{
        float time = iGlobalTime * 0.1;

        float t = DIRECTION time*.33;
        vec2 uv = fragCoord.xy / iResolution.xy-0.5;
        uv.x *= iResolution.x / iResolution.y;
        uv*= curvature* SIZE;

        float r = sqrt(dot(uv, uv)); // DISTANCE FROM CENTER, A.K.A CIRCLE
        float x = dot(MODE(uv), vec2(.5,0.))+t;
        float y = dot(MODE(uv), vec2(.0,.5))+t;

        vec4 tt = transform_VP * vec4(x,y,0,1);
        x = tt.x; y = tt.y;

    float val;
    val = fbm(vec2(MODE2 y * ray_density, MODE2 x MODE3 ray_density)); // GENERATES THE FLARING
        val = smoothstep(gamma * 0.02 - 0.1, ray_brightness + (gamma * 0.02 - 0.1) + 0.001, val);
        val = sqrt(val); // WE DON'T REALLY NEED SQRT HERE, CHANGE TO 15. * val FOR PERFORMANCE

        vec3 col = val INVERT vec3(red, green, blue);
        col = 1.-col; // WE DO NOT NEED TO CLAMP THIS LIKE THE NIMITZ SHADER DOES!
        float rad= 30. * 0.3; // MODIFY THIS TO CHANGE THE RADIUS OF THE SUNS CENTER
        col = mix(col,vec3(1.), rad - 266.667 * r); // REMOVE THIS TO SEE THE FLARING

        const vec3 W = vec3(0.2125, 0.7154, 0.0721);
        float lum = dot(col.xyz, W);
        fragColor = vec4(col, lum);
}
#endif

