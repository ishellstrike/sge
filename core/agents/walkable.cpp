#include "walkable.h"

void Walkable::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(cost));
}

std::shared_ptr<Agent> Walkable::Instantiate()
{
    return std::make_shared<Walkable>();
}

bool Walkable::IsStatic()
{
    return true;
}
