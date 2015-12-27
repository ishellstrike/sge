#include "wander.h"
#include "entity.h"
#include "random.h"
#include "core/level.h"

void Wander::onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    Creature *e = par->GetAgent<Creature>();
    std::list<std::weak_ptr<Object>> o = l->GetObjectsInRange(pos, 10);
    for(std::weak_ptr<Object> &i : o)
    {
        if(!i.lock()->base->HasAgent<Wander>())
        {
            e->current_order.type = Order::Follow;
            e->current_order.target = i;
            return;
        }
    }


//    if( e && e->current_order.type == Order::Nothing )
//    {
//        e->current_order = Order(pos + glm::vec3((random::next()-0.5f)*5,
//                                            (random::next()-0.5f)*5,
//                                            pos.z),
//                                 Order::Walk);
//    }
}
