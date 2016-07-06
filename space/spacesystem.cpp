/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "spacesystem.h"
#include <glm/gtx/vector_angle.hpp>
#include "..\helper.h"

SpaceSystem::SpaceSystem()
{

}

std::vector<SpaceSystem::system_snap> SpaceSystem::GetSystemSnap(const glm::vec3 &center, const glm::vec3 &forward, const glm::vec3 &right)
{
    std::vector<SpaceSystem::system_snap> result;
    result.reserve(system.size());
    sge::plane plane = sge::plane(center, forward, right);
    for(const auto &a : system)
    {
        SpaceSystem::system_snap snap;

        glm::vec3 apos = a->pos;
        snap.longitude = glm::orientedAngle(glm::normalize(apos - center), right, forward);
        snap.log_distance = sge::log_clamp(glm::distance(center, apos));
        snap.z_offset = -sge::log_clamp(sge::distance(plane, apos));

        result.push_back(snap);
    }

    return result;
}

