#include "helper.h"

glm::ray normalize(const glm::ray &x)
{
    return glm::ray(x.pos, normalize(x.dir));
}
