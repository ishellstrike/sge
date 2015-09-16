#ifndef ATTACHPOINT_H
#define ATTACHPOINT_H
#include <glm/glm.hpp>
#include <bitset>

class AttachPoint
{
public:
    AttachPoint();

    glm::vec3 offset;

    float size;
};

#endif // ATTACHPOINT_H
