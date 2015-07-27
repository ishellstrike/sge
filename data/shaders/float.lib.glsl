/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT License.
        See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

vec4 encodeFloat( float v ) {
  vec3 enc = vec3(1.0, 255.0, 65025.0) * v;
  enc = fract(enc);
  return vec4(enc, 1);
}

float decodeFloat( vec4 rgb ) {
  return dot(rgb.rgb, vec3(1.0, 1/255.0, 1/65025.0));
}

vec4 encodeNormal2( vec3 v )
{
    vec4 enc = vec4(vec2(1.0, 255.0) * v.x, vec2(1.0, 255.0) * v.y);
    enc = fract(enc);
    return enc;
}

vec3 decodeNormal2( vec4 rgb )
{
    return vec3(dot(rgb.rg, vec2(1.0, 1/255.0)), dot(rgb.ba, vec2(1.0, 1/255.0)), 0);
}

vec4 encodeNormal( vec3 v )
{
    return vec4(v.xyz/2 + 0.5, 1);
}

vec3 decodeNormal( vec4 enc )
{
    return vec3(enc.xyz*2 - 1);
}
