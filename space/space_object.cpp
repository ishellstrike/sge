#include "space_object.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include "helper.h"
#include "gametimer.h"

#ifndef FAST_GLM
#define sqrt glm::sqrt
#define distance glm::distance
#define rsqrt glm::inverseSqrt
#define normalize glm::normalize
#else
#define sqrt glm::fastSqrt
#define distance glm::fastDistance
#define rsqrt glm::fastInverseSqrt
#define normalize glm::fastNormalize
#endif

SpaceObject::SpaceObject()
{

}

SpaceObject::SpaceObject(float __mass, float __ro, glm::vec3 p0 /* = glm::vec(0) */) : m_mass(__mass), m_ro(__ro), pos(p0)
{
    BuildVR();
    hist.resize(max_h);
    color = glm::vec4(rand()%255/255.0f + 0.5f, rand()%255/255.0f + 0.5f, rand()%255/255.0f + 0.5f, 1.0f);
}

void SpaceObject::BuildVR()
{
    m_V = m_mass / m_ro;
    m_R = glm::pow( ( ( V() / glm::pi<double>() ) * (3.0 / 4.0) ), 1.0 / 3.0 );
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
void SpaceObject::ro(double __ro)
{
    m_ro = __ro;
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
        if(r > m_R)
            d += glm::pow( a.mass() * ( a.pos.x - local_x ) / r, 3.0 );
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
        if(r > m_R)
            d += glm::pow( a.mass() * ( a.pos.y - local_y ) / r, 3.0 );
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
        if(r > m_R)
            d += glm::pow( a.mass() * ( a.pos.y - local_z ) / r, 3.0 );
    }

    return d;
}

void SpaceObject::calcX(SpaceSystem &syst)
{
    double k1 = T * fx(pos.x, syst);
    double q1 = T * speed.x;

    double k2 = T * fx(pos.x + q1 / 2.0, syst);
    double q2 = T * (speed.x + k1 / 2.0);

    double k3 = T * fx(pos.x + q2 / 2.0, syst);
    double q3 = T * (speed.x + k2 / 2.0);

    double k4 = T * fx(pos.x + q3, syst);
    double q4 = T * (speed.x + k3);

    speed.x += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    pos.x += (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
}

void SpaceObject::calcY(SpaceSystem &syst)
{
    double k1 = T * fy(pos.y, syst);
    double q1 = T * speed.y;

    double k2 = T * fy(pos.y + q1 / 2.0, syst);
    double q2 = T * (speed.y + k1 / 2.0);

    double k3 = T * fy(pos.y + q2 / 2.0, syst);
    double q3 = T * (speed.y + k2 / 2.0);

    double k4 = T * fy(pos.y + q3, syst);
    double q4 = T * (speed.y + k3);

    speed.y += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    pos.y += (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
}

void SpaceObject::calcZ(SpaceSystem &syst)
{
    double k1 = T * fz(pos.z, syst);
    double q1 = T * speed.z;

    double k2 = T * fz(pos.z + q1 / 2.0, syst);
    double q2 = T * (speed.z + k1 / 2.0);

    double k3 = T * fz(pos.z + q2 / 2.0, syst);
    double q3 = T * (speed.z + k2 / 2.0);

    double k4 = T * fz(pos.z + q3, syst);
    double q4 = T * (speed.z + k3);

    speed.z += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    pos.z += (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
}

void SpaceObject::Update(SpaceSystem &syst, GameTimer &gt)
{
    glm::dvec3 lpos = pos;

    calcX(syst);
    calcY(syst);
    calcZ(syst);

    moving += (float)glm::abs(distance(pos, lpos));
    if(moving >= m_R)
    {
        cur_h++;
        if(cur_h >= max_h)
        {
            cur_h = 0;
        }
        hist[cur_h] = pos;
        moving = 0;
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
