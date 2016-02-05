/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "aggressive.h"
#include "core/objectstatic.h"
#include "core/db.h"
#include "core/agents/sge_sound.h"
#include "core/agents/entity.h"
#include "core/level.h"
#include "core/events/eventbus.h"
#include "core/events/eventissureorder.h"

void Aggressive::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(sound), NVP(allies));
}

void Aggressive::onUpdate(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(owner)
    {
        if( owner->current_order.type == Order::Nothing ||
            owner->current_order.type == Order::Wander )
        {
            if(agro)
            {
                agro->Move(pos);
                agro->Play();
            }

            std::list<std::weak_ptr<Object>> o = l->GetObjectsInRange(pos, 10);
            for(std::weak_ptr<Object> &i : o)
            {
                const auto &temp = i.lock();
                if(!temp->base->HasAgent<Wander>())
                {
                    owner->current_order.type = Order::Follow;
                    owner->current_order.target = i;
                    return;
                }
            }
        }
    }
}

void Aggressive::onInit(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &, const GameTimer &)
{
    owner = par->GetAgent<Creature>();
    if(!owner)
    {
        LOG(error) << "non Creature object has Aggressive agent";
    }
}

void Aggressive::onDbLoad(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &, const GameTimer &)
{
    agro = nullptr;
    if(const ObjectStatic *os = DB::Get(sound))
    {
        if(const SgeSound *o = os->GetAgent<SgeSound>())
        {
            LOG(trace) << "aggro sound " << sound << " linked";
            agro = const_cast<SgeSound*>(o);
        }
    }
}
