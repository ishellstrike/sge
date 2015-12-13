#include "entity.h"


bool Entity::IsStatic()
{
    return false;
}


void Entity::Deserialize(rapidjson::Value &val)
{

}

std::unique_ptr<Agent> Entity::Instantiate()
{
    return std::unique_ptr<Entity>(new Entity());
}
