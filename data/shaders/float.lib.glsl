vec4 unpackColor( float v ) {
  vec4 enc = vec4(1.0, 255.0, 65025.0, 0.0) * v;
  enc = fract(enc);
  enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
  return enc;
}

float packColor( vec4 rgba ) {
  return clamp(dot( rgba, vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) ), 0.0, 1.0);
}
