vec4 encodeFloat( float v ) {
  vec3 enc = vec3(1.0, 255.0, 65025.0) * v;
  enc = fract(enc);
  return vec4(enc, 1);
}

float decodeFloat( vec4 rgb ) {
  return dot(rgb.rgb, vec3(1.0, 1/255.0, 1/65025.0));
}

vec4 encodeNormal( vec3 v )
{
    return vec4(v.xyz/2 + 0.5, 0);
}

vec3 decodeNormal( vec4 enc )
{
    return vec3(enc.xyz*2 - 1);
}
