/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "helper.h"

sge::ray normalize(const sge::ray &x)
{
    return sge::ray(x.origin, normalize(x.dir));
}


void sge::ray::precompute()
{
    dir = normalize(dir);
    inv = glm::vec3(1/dir.x, 1/dir.y, 1/dir.z);
    sign[0] = inv.x < 0;
    sign[1] = inv.y < 0;
    sign[2] = inv.z < 0;
}

bool sge::intersect(const sge::ray &r, float t0, float t1, glm::vec3 min, glm::vec3 max)
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


void Bresencham3D(const glm::vec3 &p1, const glm::vec3 &p2, std::vector<glm::vec3> &out_points)
{
    int x  = static_cast<int>(p1.x);
    int y  = static_cast<int>(p1.y);
    int z  = static_cast<int>(p1.z);
    int dx = static_cast<int>(glm::abs(p2.x - x));
    int dy = static_cast<int>(glm::abs(p2.y - y));
    int dz = static_cast<int>(glm::abs(p2.z - z));
    int sx = static_cast<int>(glm::sign(p2.x - x));
    int sy = static_cast<int>(glm::sign(p2.y - y));
    int sz = static_cast<int>(glm::sign(p2.z - z));

    if( (dy >= dx) && (dy >= dz) )
    {
        register int e_yx = (dx - dy) << 1;
        register int e_yz = (dz - dy) << 1;

        e_yx -= (e_yx >> 1);
        e_yz -= (e_yz >> 1);

        for(register int i = 0; i < dy; ++i)
        {
            out_points.push_back(glm::vec3(x, y, z));
            if(e_yx >= 0)
            {
                x += sx;
                e_yx -= (dy << 1);
                out_points.push_back(glm::vec3(x, y, z));
            }

            if(e_yz >= 0)
            {
                z += sz;
                e_yz -= (dy << 1);
                out_points.push_back(glm::vec3(x, y, z));
            }

            y += sy;
            e_yx += (dx << 1);
            e_yz += (dz << 1);
        }
    }
    else if( (dx >= dy) && (dx >= dz) )
    {
        register int e_xy = (dy - dx) << 1;
        register int e_xz = (dz - dx) << 1;

        e_xz -= (e_xz >> 1);
        e_xy -= (e_xy >> 1);

        for(register int i = 0; i < dx; ++i)
        {
            out_points.push_back(glm::vec3(x, y, z));

            if(e_xy>=0)
            {
                y += sy;
                e_xy -= (dx << 1);
                out_points.push_back(glm::vec3(x, y, z));
            }

            if(e_xz>=0)
            {
                z += sz;
                e_xz -= (dx << 1);
                out_points.push_back(glm::vec3(x, y, z));
            }
            x += sx;
            e_xy +=(dy << 1);
            e_xz +=(dz << 1);
        }
    }
    else // (dz>=dy) && (dz>=dx)
    {
        register int e_zx = (dx - dz) << 1;
        register int e_zy = (dy - dz) << 1;

        e_zx -= (e_zx >> 1);
        e_zy -= (e_zy >> 1);

        for(register int i = 0; i < dz; ++i)
        {
            out_points.push_back(glm::vec3(x, y, z));

            if(e_zx>=0)
            {
                x += sx;
                e_zx -= (dz << 1);
                out_points.push_back(glm::vec3(x, y, z));
            }
            if(e_zy>=0)
            {
                y += sy;
                e_zy -= (dz << 1);
                out_points.push_back(glm::vec3(x, y, z));
            }

            z += sz;
            e_zx += (dx << 1);
            e_zy += (dy << 1);
        }


    }
    out_points.push_back(glm::vec3(x, y, z));
}




namespace sge {

plane::plane() : a(0), b(0), c(0), d(0)
{

}

plane::plane(const glm::vec3 &center, const glm::vec3 &vec1, const glm::vec3 &vec2)
{
    glm::vec3 normal = glm::cross(vec1, vec2);

    glm::vec3 n = glm::normalize(normal);
    a = n.x;
    b = n.y;
    c = n.z;
    d = -glm::dot(center, n);
}

plane::plane(const glm::vec3& normal, const glm::vec3& point)
{
    glm::vec3 n = glm::normalize(normal);
    a = n.x;
    b = n.y;
    c = n.z;
    d = -glm::dot(point, n);
}

plane::plane(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d)
{
}

plane plane::normalize(const plane &pl)
{
    plane res;
    float dist = glm::sqrt(pl.a * pl.a + pl.b * pl.b + pl.c * pl.c);
    res.a = pl.a / dist;
    res.b = pl.b / dist;
    res.c = pl.c / dist;
    res.d = pl.d / dist;
    return res;
}

void plane::normalize_this()
{
    plane t = normalize(*this);

    a = t.a;
    b = t.b;
    c = t.c;
    d = t.d;
}

glm::vec3 plane::closest_point(const glm::vec3 &point) const
{
    return (point - normal() * distance(point));
}

float plane::distance(const glm::vec3 &point) const
{
    return (a * point.x + b * point.y + c * point.z + d);
}

glm::vec3 plane::normal() const
{
    return glm::vec3(a, b, c);
}

float distance(const plane &pl, const glm::vec3 &point)
{
    return (pl.a * point.x + pl.b * point.y + pl.c * point.z + pl.d);
}

}
