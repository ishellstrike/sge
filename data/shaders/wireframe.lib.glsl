float edgeFactor(in vec3 norm)
{

    float thickness = 0.1;

    vec3 d = norm;
    vec3 a3 = mix(vec3(0.0, 0.0, 0.0), (d) + 4.0, norm);
    float th = min(min(a3.x, a3.y), a3.z );

    float r = 0.0;

    if(th > thickness)
        r = 1.0;
    return r;
}
