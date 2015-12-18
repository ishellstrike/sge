#include "entity.h"


void Entity::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Entity::Instantiate()
{
    return std::make_shared<Entity>();
}

bool Entity::IsStatic()
{
    return false;
}
