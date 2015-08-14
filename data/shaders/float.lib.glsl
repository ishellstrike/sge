/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

vec4 encodeFloat( in float depth ) {
    // See Aras Pranckevičius' post Encoding Floats to RGBA
    // http://aras-p.info/blog/2009/07/30/encoding-floats-to-rgba-the-final/
    vec4 enc = vec4(1.0, 255.0, 65025.0, 16581375.0) * depth;
    enc = fract(enc);
    enc -= enc.yzww * vec4(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 0.0);
    enc.w = 1;
    return enc;
}

float decodeFloat( in vec4 packedDepth ) {
    // See Aras Pranckevičius' post Encoding Floats to RGBA
    // http://aras-p.info/blog/2009/07/30/encoding-floats-to-rgba-the-final/
    return dot(packedDepth, vec4(1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 16581375.0));
}

vec4 encodeNormal1( in vec3 v )
{
    vec4 enc = vec4(vec2(1.0, 255.0) * v.x, vec2(1.0, 255.0) * v.y);
    enc = fract(enc);
    return enc;
}

vec3 decodeNormal1( in vec4 rgb )
{
    return vec3(dot(rgb.rg, vec2(1.0, 1/255.0)), dot(rgb.ba, vec2(1.0, 1/255.0)), 0);
}

vec4 encodeNormal( in vec3 v )
{
    return vec4(v.xyz/2 + 0.5, 1);
}

vec3 decodeNormal( in vec4 enc )
{
    return vec3(enc.xyz*2 - 1);
}
