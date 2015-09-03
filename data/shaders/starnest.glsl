/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#define iterations 17
#define formuparam 0.50

#define volsteps 20
#define stepsize 0.1

#define zoom   1.800
#define tile   0.850
#define speed  0.010

#define brightness 0.0015
#define darkmatter 0.0300
#define distfading 0.730
#define saturation 0.850

uniform mat4 transform_VP; // view * projection matrix

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

const vec2 iResolution = vec2(1,1);
const vec2 iMouse = vec2(0);
float iGlobalTime = 0;

void main(void)
{
    // Star Nest by Pablo Román Andrioli
    // This content is under the MIT License.

    //get coords and direction
    vec2 uv = fragCoord.xy / iResolution.xy - 0.5;
    uv.y *= iResolution.y / iResolution.x;
    vec3 dir = (transform_VP * vec4(1,0,0,1)).xyz - vec3(uv, 0);
    float time = iGlobalTime * speed + 0.25;

    vec3 from = (transform_VP * vec4(0, 0, 0, 1)).xyz;

    //volumetric rendering
    float s = 0.1, fade = 1;
    vec3 v = vec3(0);
    for (int r = 0; r < volsteps; r++) {
            vec3 p = from + s * dir * 0.5;
            p = abs(vec3(tile) - mod(p, vec3(tile * 2))); // tiling fold
            float pa, a = pa = 0;
            for (int i=0; i < iterations; i++) {
                    p = abs(p) / dot(p, p) - formuparam; // the magic formula
                    a += abs(length(p) - pa); // absolute sum of average change
                    pa = length(p);
            }
            float dm = max(0, darkmatter - a * a * 0.001); //dark matter
            a *= a * a; // add contrast
            if (r > 6) fade *= 1 - dm; // dark matter, don't render near
            //v+=vec3(dm,dm*.5,0.);
            v += fade;
            v += vec3(s, s * s, s * s * s * s) * a * brightness * fade; // coloring based on distance
            fade *= distfading; // distance fading
            s += stepsize;
    }
    v = mix(vec3(length(v)), v, saturation); //color adjust
    fragColor = vec4(v * 0.01, 1);
}
#endif
