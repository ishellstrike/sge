#include "helper.h"

glm::ray normalize(const glm::ray &x)
{
    return glm::ray(x.origin, normalize(x.dir));
}


void glm::ray::precompute()
{
    dir = normalize(dir);
    inv = glm::vec3(1/dir.x, 1/dir.y, 1/dir.z);
    sign[0] = inv.x < 0;
    sign[1] = inv.y < 0;
    sign[2] = inv.z < 0;
}

bool glm::intersect(const glm::ray &r, float t0, float t1, glm::vec3 min, glm::vec3 max)
{
    glm::vec3 bounds[2] = {min, max};
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[r.sign[0]].x - r.origin.x) * r.inv.x;
    tmax = (bounds[1-r.sign[0]].x - r.origin.x) * r.inv.x;
    tymin = (bounds[r.sign[1]].y - r.origin.y) * r.inv.y;
    tymax = (bounds[1-r.sign[1]].y - r.origin.y) * r.inv.y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[r.sign[2]].z - r.origin.z) * r.inv.z;
    tzmax = (bounds[1-r.sign[2]].z - r.origin.z) * r.inv.z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    return ((tmin < t1) && (tmax > t0));
}
