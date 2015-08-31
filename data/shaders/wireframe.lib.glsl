float edgeFactor(in vec3 norm, in float thickness)
{
    vec3 d = norm;
    vec3 a3 = mix(vec3(0.0, 0.0, 0.0), (d) + 4.0, norm);
    float th = min(min(a3.x, a3.y), a3.z );

    float r = 0.0;

    if(th > thickness)
        r = 1.0;
    return r;
}

float edgeFactor(in vec2 norm, in float thickness)
{
    float r = 0;
    if(norm.x < thickness || norm.y < thickness)
        r = 1.0;

    if(norm.x > 1 - thickness || norm.y > 1 - thickness)
        r = 1.0;
    return r;
}
