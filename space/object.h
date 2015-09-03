/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SPACE_OBJECT_H
#define SPACE_OBJECT_H
#include <glm/glm.hpp>
#include <glm/gtx/common.hpp>
#include "spacesystem.h"
#include "gametimer.h"
#include "solver.h"
#include "experimental/quadsphere.h"
#include "camera.h"

class Object
{
public:
    Object();
    Object(float __mass, float __ro, glm::vec3 p0 = glm::vec3(0));


    glm::dvec3 pos = glm::dvec3(0), speed = glm::dvec3(0), acc = glm::dvec3(0);
    std::vector<glm::dvec3> hist;
    float last = 0;
    float moving = 0;
    bool dominant = false;

    glm::vec4 color;
    const int max_h = 1000;
    int cur_h = 0;

    double ro() const; /*< density kg/m^3*/
    void ro(double __ro);
    double ro_si() const; /*< density g/cm^3*/
    void ro_si(double __ro);

    template <typename _Ty = double>
    _Ty V() const /*< volume (indirect)*/
    {
        return _Ty(m_V);
    }

    template <typename _Ty = double>
    _Ty R() const /*< radius (indirect)*/
    {
        return _Ty(m_R);
    }

    template <typename _Ty = double>
    _Ty mass() const /*< mass*/
    {
        return _Ty(m_mass);
    }


    void mass(double __mass);

    double fx(double local_x, SpaceSystem &syst);
    double fy(double local_y, SpaceSystem &syst);
    double fz(double local_y, SpaceSystem &syst);
    void rk4_x(SpaceSystem &syst, GameTimer &);
    void rk4_y(SpaceSystem &syst, GameTimer &);
    void rk4_z(SpaceSystem &syst, GameTimer &);
    const double T = 0.00003;

    std::string GetDebugInfo();

    virtual void Update(SpaceSystem &syst, GameTimer &gt, const Camera &cam);
    virtual void Render(Camera &camera) const;

private:
    double m_mass = 1.0;
    double m_R = 1.0;
    double m_ro = 1.0;
    double m_V = 1.0;

    void BuildVR();
};

class Planet : public Object
{
public:
    Planet(float __mass, float __ro, glm::vec3 p0 = glm::vec3(0));

    void InitRender(std::shared_ptr<Material> &__mat);
    std::unique_ptr<QuadSphere> render;

    void Render(Camera &camera) const override;
    void Update(SpaceSystem &syst, GameTimer &gt, const Camera &cam) override;
};

#endif // SPACE_OBJECT_H
