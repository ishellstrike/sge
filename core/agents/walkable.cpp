#include "walkable.h"

void Walkable::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(cost));
}
