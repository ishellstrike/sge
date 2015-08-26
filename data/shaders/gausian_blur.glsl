/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
uniform sampler2D mainMap;

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

void main( void )
{
    vec2 tx  = fragCoord;
    vec2 dx  = vec2 (0.001953,0.000000);
    vec2 sdx = dx;
    vec4 sum = texture(mainMap, tx) * 0.134598;

    sum += (texture(mainMap, tx + sdx) + texture(mainMap, tx - sdx))* 0.127325;
    sdx += dx;
    sum += (texture(mainMap, tx + sdx) + texture(mainMap, tx - sdx))* 0.107778;
    sdx += dx;
    sum += (texture(mainMap, tx + sdx) + texture(mainMap, tx - sdx))* 0.081638;
    sdx += dx;
    sum += (texture(mainMap, tx + sdx) + texture(mainMap, tx - sdx))* 0.055335;
    sdx += dx;
    sum += (texture(mainMap, tx + sdx) + texture(mainMap, tx - sdx))* 0.033562;
    sdx += dx;
    sum += (texture(mainMap, tx + sdx) + texture(mainMap, tx - sdx))* 0.018216;
    sdx += dx;
    sum += (texture(mainMap, tx + sdx) + texture(mainMap, tx - sdx))* 0.008847;
    sdx += dx;

    fragColor = sum;
}
#endif
