#include "playeremplacer.h"

PlayerEmplacer::PlayerEmplacer()
{
    id = rand();
}

PlayerEmplacer::PlayerEmplacer(glm::vec3 __pos, float __look, size_t __id) :
    pos(__pos),
    look(__look),
    id(__id)
{

}

