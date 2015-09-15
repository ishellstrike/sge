#ifndef HULL_H
#define HULL_H
#include <glm/glm.hpp>
#include <list>
#include "attachpoint.h"

class Hull
{
public:
    Hull();

    glm::vec3 size;
    std::list<AttachPoint> attaches;
};

#endif // HULL_H
