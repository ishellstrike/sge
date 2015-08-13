/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT License.
        See "license.txt"
*******************************************************************************/

#include "turbulence.lib.glsl"

float ground(vec2 uv, out vec3 grad)
{
    //vec3 grad1;
    //vec3 grad2;
    //float a = ((snoise( 5 * position, grad1 )*5 + snoise( 100 * position, grad2 ))/6.0);
    grad = vec3(1,1,1);//(grad1*5 + grad2)/6.0f;
    return iqTurbulence(uv, 50, 3);;
}
