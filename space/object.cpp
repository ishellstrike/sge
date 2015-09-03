/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "object.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include "helper.h"
#include "gametimer.h"
#include <random>

#define ZERO_FIX

#ifdef ZERO_FIX
#define if_not_zero_r if(r > m_R + a.m_R)
#else
#define is_not_zero_distance
#endif

Object::Object()
{

}

/*!
 * \brief SpaceObject::SpaceObject
 * \param __mass mass in kg
 * \param __ro density in kg/m^3
 * \param p0
 */
Object::Object(float __mass, float __ro, glm::vec3 p0 /* = glm::vec(0) */) : m_mass(__mass), m_ro(__ro), pos(p0)
{
    BuildVR();
    hist.resize(max_h);
    color = glm::vec4(random::next<float>() + 0.5f, random::next<float>() + 0.5f, random::next<float>() + 0.5f, 1.0f);
}

void Planet::InitRender(std::shared_ptr<Material> &__mat)
{
    render = std::unique_ptr<QuadSphere>(new QuadSphere(__mat));
}

void Object::BuildVR()
{
    m_V = m_mass / m_ro;
    m_R = pow( ( ( ( 3.0 * V() ) / (4.0 * glm::pi<double>() ) ) ), 1.0 / 3.0 );
}

void Object::mass(double __mass)
{
    m_mass = __mass;
    BuildVR();
}

double Object::ro() const
{
    return m_ro;
}
double Object::ro_si() const
{
    return m_ro / 1000.0;
}
void Object::ro(double __ro)
{
    m_ro = __ro;
    BuildVR();
}
void Object::ro_si(double __ro)
{
    m_ro = __ro * 1000.0;
    BuildVR();
}

double Object::fx(double local_x, SpaceSystem &syst)
{
    double d = 0;
    for (auto A : syst.system)
    {
        if(A.get() == this) continue;
        Object &a = *A;
        double dx = a.pos.x - local_x;
        double dy = a.pos.y - pos.y;
        double dz = a.pos.z - pos.z;
        double r = glm::sqrt( dx*dx + dy*dy + dz*dz );
        if_not_zero_r
            d +=  a.mass() * ( a.pos.x - local_x ) / pow( r, 3.0 );
    }

    return d;
}

double Object::fy(double local_y, SpaceSystem &syst)
{
    double d = 0;
    for (auto A : syst.system)
    {
        if(A.get() == this) continue;
        Object &a = *A;
        double dx = a.pos.x - pos.x;
        double dy = a.pos.y - local_y;
        double dz = a.pos.z - pos.z;
        double r = glm::sqrt( dx*dx + dy*dy + dz*dz );
        if_not_zero_r
            d +=  a.mass() * ( a.pos.y - local_y ) / pow( r, 3.0 );
    }

    return d;
}

double Object::fz(double local_z, SpaceSystem &syst)
{
    double d = 0;
    for (auto A : syst.system)
    {
        if(A.get() == this) continue;
        Object &a = *A;
        double dx = a.pos.x - pos.x;
        double dy = a.pos.y - pos.y;
        double dz = a.pos.z - local_z;
        double r = glm::sqrt( dx*dx + dy*dy + dz*dz );
        if_not_zero_r
            d +=  a.mass() * ( a.pos.z - local_z ) / pow( r, 3.0 );
    }

    return d;
}

void Object::rk4_x(SpaceSystem &syst, GameTimer &)
{
    double k1 = T * fx(pos.x, syst);
    double q1 = T * speed.x;

    double k2 = T * fx(pos.x + q1 / 2.0, syst);
    double q2 = T * (speed.x + k1 / 2.0);

    double k3 = T * fx(pos.x + q2 / 2.0, syst);
    double q3 = T * (speed.x + k2 / 2.0);

    double k4 = T * fx(pos.x + q3, syst);
    double q4 = T * (speed.x + k3);

    acc.x = (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    speed.x += acc.x;
    pos.x += (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
}

void Object::rk4_y(SpaceSystem &syst, GameTimer &)
{
    double k1 = T * fy(pos.y, syst);
    double q1 = T * speed.y;

    double k2 = T * fy(pos.y + q1 / 2.0, syst);
    double q2 = T * (speed.y + k1 / 2.0);

    double k3 = T * fy(pos.y + q2 / 2.0, syst);
    double q3 = T * (speed.y + k2 / 2.0);

    double k4 = T * fy(pos.y + q3, syst);
    double q4 = T * (speed.y + k3);

    acc.y = (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    speed.y += acc.y;
    pos.y += (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
}

void Object::rk4_z(SpaceSystem &syst, GameTimer &)
{
    double k1 = T * fz(pos.z, syst);
    double q1 = T * speed.z;

    double k2 = T * fz(pos.z + q1 / 2.0, syst);
    double q2 = T * (speed.z + k1 / 2.0);

    double k3 = T * fz(pos.z + q2 / 2.0, syst);
    double q3 = T * (speed.z + k2 / 2.0);

    double k4 = T * fz(pos.z + q3, syst);
    double q4 = T * (speed.z + k3);

    acc.z = (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    speed.z += acc.z;
    pos.z += (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
}

void Object::Update(SpaceSystem &syst, GameTimer &gt, const Camera &cam)
{
    glm::dvec3 lpos = pos;

    if(!dominant)
    {
        rk4_x(syst, gt);
        rk4_y(syst, gt);
        rk4_z(syst, gt);
    }

    hist[cur_h] = pos;

    last += gt.elapsed;
    //moving += (float)glm::abs(distance(pos, lpos));
    //if(moving >= m_R*2)
    if(last >= 1)
    {
        cur_h++;
        if(cur_h >= max_h)
        {
            cur_h = 0;
        }
        moving = 0;
        last = 0;
    }
}

void Object::Render(Camera &camera) const
{

}

/*!
 * \brief SpaceObject::SpaceObject
 * \param __mass mass in kg
 * \param __ro density in kg/m^3
 * \param p0
 */
Planet::Planet(float __mass, float __ro, glm::vec3 p0 /* = glm::vec(0) */) : Object(__mass, __ro, p0)
{
}

void Planet::Render(Camera &camera) const
{
    if(render)
    {
        render->Render(camera);
    }
}

void Planet::Update(SpaceSystem &syst, GameTimer &gt, const Camera &cam)
{
    Object::Update(syst, gt, cam);
    if(render)
    {
        render->world = glm::translate(glm::mat4(1), glm::vec3(pos));
        render->s = R<float>()/10.0f;
        render->R = R<float>();
        render->center = glm::vec3(pos);
        render->Update(cam);
    }
}

std::string Object::GetDebugInfo()
{
    return string_format("pos:%s\nv:%s\na:%s\nmass:%g\ndensiry:%g\nvolume:%g\nradius:%g",
                         std::to_string(pos).c_str(),
                         std::to_string(speed).c_str(),
                         std::to_string(acc).c_str(),
                         m_mass,
                         m_ro,
                         m_V,
                         m_R);
}

#undef ZERO_FIX
#undef if_not_zero_r
