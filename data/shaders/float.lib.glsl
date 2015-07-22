float packColor(vec3 color) {
    return (color.r + color.g * 256.0 + color.b * 256.0 * 256.0)/16777215.0;
}

vec3 unpackColor(float mf) {
    float f = mf * 16777215.0;
    vec3 color;
    color.b = floor(f / 256.0 / 256.0);
    color.g = floor((f - color.b * 256.0 * 256.0) / 256.0);
    color.r = floor(f - color.b * 256.0 * 256.0 - color.g * 256.0);
    return color / 256.0;
}
