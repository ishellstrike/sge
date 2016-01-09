/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "wander.h"
#include "entity.h"
#include "random.h"
#include "core/level.h"

void Wander::onUpdate(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    Creature *e = par->GetAgent<Creature>();
    if( e && e->current_order.type == Order::Nothing )
    {
        e->current_order = Order(pos + glm::vec3((random::next()-0.5f)*5,
                                            (random::next()-0.5f)*5,
                                            pos.z),
                                 Order::Wander);
    }
}
