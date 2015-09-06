#ifndef AABB_H
#define AABB_H
#include <glm/glm.hpp>

struct AABB
{
    glm::vec3 min, max;
    bool builded = false;
};

#endif // AABB_H

