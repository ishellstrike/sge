#include "entity.h"
#include "core\level.h"

void Entity::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Entity::Instantiate() const
{
    return std::make_shared<Entity>();
}

void Entity::onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(current_order.type != Order::Nothing)
    {
        l->DeltaEntity( *par, glm::normalize(current_order.pos - pos)*gt.elapsed );

        if(glm::round(current_order.pos) == glm::round(pos))
            current_order.type = Order::Nothing;
    }
}
