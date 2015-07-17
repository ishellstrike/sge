float ground(vec3 position, out vec3 grad)
{
    vec3 grad1;
    vec3 grad2;
    float a = ((snoise( 5 * position, grad1 )*5 + snoise( 100 * position, grad2 ))/6.0);
    grad = (grad1*5 + grad2)/6.0f;
    return a;
}
