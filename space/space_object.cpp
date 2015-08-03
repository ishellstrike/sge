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

void SpaceObject::InitRender(std::shared_ptr<BasicJargShader> &shader, std::shared_ptr<Material> &__mat)
{
    render = std::unique_ptr<QuadSphere>(new QuadSphere(shader, __mat));
}

void SpaceObject::BuildVR()
{
    m_V = m_mass / m_ro;
    m_R = pow( ( ( ( 3.0 * V() ) / (4.0 * glm::pi<double>() ) ) ), 1.0 / 3.0 );
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
        //if(r > m_R + a.m_R)
            d +=  a.mass() * ( a.pos.x - local_x ) / pow( r, 3.0 );
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
        //if(r > m_R + a.m_R)
            d +=  a.mass() * ( a.pos.y - local_y ) / pow( r, 3.0 );
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
        //if(r > m_R + a.m_R)
            d +=  a.mass() * ( a.pos.z - local_z ) / pow( r, 3.0 );
    }

    return d;
}

void SpaceObject::rk4_x(SpaceSystem &syst, GameTimer &)
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

void SpaceObject::rk4_y(SpaceSystem &syst, GameTimer &)
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

void SpaceObject::rk4_z(SpaceSystem &syst, GameTimer &)
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

void SpaceObject::Update(SpaceSystem &syst, GameTimer &gt, const Camera &cam)
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

    if(render)
    {
        render->world = glm::translate(glm::mat4(1), glm::vec3(pos));
        render->s = R<float>()/10.0;
        render->R = R<float>();
        render->center = glm::vec3(pos);
        render->Update(cam);
    }
}

void SpaceObject::Render(Camera &camera)
{
    if(render)
    {
        render->Render(camera);
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
