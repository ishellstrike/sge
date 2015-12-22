#include "entity.h"


void Entity::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Entity::Instantiate() const
{
    return std::make_shared<Entity>();
}
