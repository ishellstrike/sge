/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#include "objectbase.h"
#include <glm/gtc/constants.hpp>

ObjectBase::ObjectBase()
{

}

ObjectBase::ObjectBase(float __mass, float __ro, glm::vec3 p0) : m_ro(__ro), pos(p0), m_mass(__mass)
{

}

void ObjectBase::mass(double __mass)
{
    m_mass = __mass;
    BuildVR();
}

void ObjectBase::BuildVR()
{
    m_V = m_mass / m_ro;
    m_R = pow( ( ( ( 3.0 * V() ) / (4.0 * glm::pi<double>() ) ) ), 1.0 / 3.0 );
}

