#include "space_object.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include "helper.h"
#include "gametimer.h"
#include <random>

SpaceObject::SpaceObject()
{

}

/*!
 * \brief SpaceObject::SpaceObject
 * \param __mass mass in kg
 * \param __ro density in kg/m^3
 * \param p0
 */
SpaceObject::SpaceObject(float __mass, float __ro, glm::vec3 p0 /* = glm::vec(0) */) : m_mass(__mass), m_ro(__ro), pos(p0)
{
    BuildVR();
    hist.resize(max_h);
    color = glm::vec4(random::next<float>() + 0.5f, random::next<float>() + 0.5f, random::next<float>() + 0.5f, 1.0f);
}

void SpaceObject::BuildVR()
{
    m_V = m_mass / m_ro;
    m_R = pow( ( ( ( 3.0 * V() ) / (4.0 * glm::pi<double>() ) ) ), 1.0 / 3.0 );
}


double SpaceObject::V() const
{
    return m_V;
}

double SpaceObject::R() const
{
    return m_R;
}

double SpaceObject::mass() const
{
    return m_mass;
}
void SpaceObject::mass(double __mass)
{
    m_mass = __mass;
    BuildVR();
}

double SpaceObject::ro() const
{
    return m_ro;
}
double SpaceObject::ro_si() const
{
    return m_ro / 1000.0;
}
void SpaceObject::ro(double __ro)
{
    m_ro = __ro;
    BuildVR();
}
void SpaceObject::ro_si(double __ro)
{
    m_ro = __ro * 1000.0;
    BuildVR();
}

double SpaceObject::fx(double local_x, SpaceSystem &syst)
{
    double d = 0;
    for (auto A : syst.system)
    {
        if(A.get() == this) continue;
        SpaceObject &a = *A;
        double dx = a.pos.x - local_x;
        double dy = a.pos.y - pos.y;
        double dz = a.pos.z - pos.z;
        double r = sqrt( dx*dx + dy*dy + dz*dz );
      //  if(r > m_R + a.m_R)
            d += pow( a.mass() * ( a.pos.x - local_x ) / r, 3.0 );
    }

    return d;
}

double SpaceObject::fy(double local_y, SpaceSystem &syst)
{
    double d = 0;
    for (auto A : syst.system)
    {
        if(A.get() == this) continue;
        SpaceObject &a = *A;
        double dx = a.pos.x - pos.x;
        double dy = a.pos.y - local_y;
        double dz = a.pos.z - pos.z;
        double r = sqrt( dx*dx + dy*dy + dz*dz );
      //  if(r > m_R + a.m_R)
            d += pow( a.mass() * ( a.pos.y - local_y ) / r, 3.0 );
    }

    return d;
}

double SpaceObject::fz(double local_z, SpaceSystem &syst)
{
    double d = 0;
    for (auto A : syst.system)
    {
        if(A.get() == this) continue;
        SpaceObject &a = *A;
        double dx = a.pos.x - pos.x;
        double dy = a.pos.y - pos.y;
        double dz = a.pos.z - local_z;
        double r = sqrt( dx*dx + dy*dy + dz*dz );
      //  if(r > m_R + a.m_R)
            d += pow( a.mass() * ( a.pos.y - local_z ) / r, 3.0 );
    }

    return d;
}

void SpaceObject::rk4_x(SpaceSystem &syst, GameTimer &gt)
{
    double k1 = gt.elapsed * T * fx(pos.x, syst);
    double q1 = gt.elapsed * T * speed.x;

    double k2 = gt.elapsed * T * fx(pos.x + q1 / 2.0, syst);
    double q2 = gt.elapsed * T * (speed.x + k1 / 2.0);

    double k3 = gt.elapsed * T * fx(pos.x + q2 / 2.0, syst);
    double q3 = gt.elapsed * T * (speed.x + k2 / 2.0);

    double k4 = gt.elapsed * T * fx(pos.x + q3, syst);
    double q4 = gt.elapsed * T * (speed.x + k3);

    speed.x += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    pos.x += (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
}

void SpaceObject::rk4_y(SpaceSystem &syst, GameTimer &gt)
{
    double k1 = gt.elapsed * T * fy(pos.y, syst);
    double q1 = gt.elapsed * T * speed.y;

    double k2 = gt.elapsed * T * fy(pos.y + q1 / 2.0, syst);
    double q2 = gt.elapsed * T * (speed.y + k1 / 2.0);

    double k3 = gt.elapsed * T * fy(pos.y + q2 / 2.0, syst);
    double q3 = gt.elapsed * T * (speed.y + k2 / 2.0);

    double k4 = gt.elapsed * T * fy(pos.y + q3, syst);
    double q4 = gt.elapsed * T * (speed.y + k3);

    speed.y += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    pos.y += (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
}

void SpaceObject::rk4_z(SpaceSystem &syst, GameTimer &gt)
{
    double k1 = gt.elapsed * T * fz(pos.z, syst);
    double q1 = gt.elapsed * T * speed.z;

    double k2 = gt.elapsed * T * fz(pos.z + q1 / 2.0, syst);
    double q2 = gt.elapsed * T * (speed.z + k1 / 2.0);

    double k3 = gt.elapsed * T * fz(pos.z + q2 / 2.0, syst);
    double q3 = gt.elapsed * T * (speed.z + k2 / 2.0);

    double k4 = gt.elapsed * T * fz(pos.z + q3, syst);
    double q4 = gt.elapsed * T * (speed.z + k3);

    speed.z += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    pos.z += (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
}

void SpaceObject::Update(SpaceSystem &syst, GameTimer &gt)
{
    glm::dvec3 lpos = pos;

        rk4_x(syst, gt);
        rk4_y(syst, gt);
        rk4_z(syst, gt);

    hist[cur_h] = pos;

    last += gt.elapsed;
    //moving += (float)glm::abs(distance(pos, lpos));
    //if(moving >= m_R*2)
    if(last >= 0.2)
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

std::string SpaceObject::GetDebugInfo()
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
