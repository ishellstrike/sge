#include "entity.h"
#include "core\level.h"

void Creature::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Creature::Instantiate() const
{
    Creature *e = new Creature();
    e->pos = pos;
    e->current_order = current_order;
    return std::shared_ptr<Creature>(e);
}

void Creature::onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(current_order.target.expired())
    {
        current_order.type = Order::Nothing;
    }

    switch(current_order.type)
    {
        case Order::Delay:
            current_order.time += gt.elapsed;
            if(current_order.time > 1)
            {
                current_order.time = 0;
                current_order.type = Order::Nothing;
            }
            break;
        case Order::Wander:
            l->DeltaCreature( *par, glm::normalize(current_order.pos - pos)*gt.elapsed );
            if(glm::round(current_order.pos) == glm::round(pos))
                current_order.type = Order::Delay;
            break;
        case Order::Follow:
            auto tpos = current_order.target.lock()->GetAgent<Creature>()->pos;
            if(glm::distance(current_order.pos, pos) < 1)
            {
                current_order.type = Order::Delay;
                break;
            }
            l->DeltaCreature( *par, glm::normalize( tpos - pos )*gt.elapsed );
            break;
    }
}
