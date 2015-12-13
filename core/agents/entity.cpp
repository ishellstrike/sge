#include "entity.h"


bool Entity::IsStatic()
{
    return false;
}


void Entity::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Entity::Instantiate()
{
    return std::unique_ptr<Entity>(new Entity());
}
