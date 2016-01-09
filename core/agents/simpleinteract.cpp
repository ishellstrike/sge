/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "simpleinteract.h"
#include "core/objectstatic.h"
#include "core/db.h"
#include "core/level.h"

void SimpleInteract::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(afterid), NVP(sound));
}

void SimpleInteract::onInteract(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &)
{
    if(activate)
    {
        activate->Move(pos);
        activate->Play();
    }
    l->SetBlockAtPos(pos, DB::Create(afterid));
}

void SimpleInteract::onDbLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &, const GameTimer&)
{
    activate = nullptr;
    if(const ObjectStatic *os = DB::Get(sound))
    {
        if(const Sound *o = os->GetAgent<Sound>())
        {
            LOG(trace) << "interact sound " << sound << " linked";
            activate = o;
        }
    }
}
