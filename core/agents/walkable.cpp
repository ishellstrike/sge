#include "walkable.h"

void Walkable::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(cost));
}
