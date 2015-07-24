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
    float p = sqrt(v.z*8 + 8);
    return vec4(v.xy/p + 0.5, 0, 0);
}

vec3 decodeNormal( vec4 enc )
{
    vec2 fenc = enc.xy*4 - 2;
    float f = dot(fenc, fenc);
    float g = sqrt(1 - f/4);
    vec3 n;
    n.xy = fenc*g;
    n.z = 1 - f/2;
    return n;
}
