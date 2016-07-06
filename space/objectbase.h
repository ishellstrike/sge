/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#ifndef OBJECTBASE_H
#define OBJECTBASE_H
#include "glm/glm.hpp"

class SpaceSystem;
class Camera;
class GameTimer;

class ObjectBase
{
public:
    ObjectBase();
    ObjectBase(float __mass, float __ro, glm::vec3 p0  = glm::vec3(0) );

    glm::dvec3 pos = glm::dvec3(0), speed = glm::dvec3(0), acc = glm::dvec3(0);

    virtual void Update(SpaceSystem &syst, GameTimer &gt, const Camera &cam) = 0;
    virtual void Render(Camera &camera) const;


    void mass(double __mass);

    double m_R = 1.0;
    double m_ro = 1.0;
    double m_V = 1.0;

    template <typename _Ty = double>
    _Ty V() const
    {
        return _Ty(m_V);
    }

    template <typename _Ty = double>
    _Ty R() const
    {
        return _Ty(m_R);
    }

    template <typename _Ty = double>
    _Ty mass() const
    {
        return _Ty(m_mass);
    }

private:
    double m_mass = 1.0;

protected:

    void BuildVR();
};

#endif // OBJECTBASE_H
