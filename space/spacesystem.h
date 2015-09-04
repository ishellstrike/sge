/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SPACESYSTEM_H
#define SPACESYSTEM_H
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "objectbase.h"

class SpaceSystem
{
public:
    SpaceSystem();
    std::vector<std::shared_ptr<ObjectBase>> system;

    struct system_snap
    {
        float z_offset;
        float longitude;
        float log_distance;
    };

    std::vector<system_snap> GetSystemSnap(const glm::vec3 &center, const glm::vec3 &forward, const glm::vec3 &right);
};

#endif // SPACESYSTEM_H
