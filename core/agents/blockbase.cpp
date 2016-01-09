/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "blockbase.h"
#include "core/db.h"
#include "material.h"

void BlockBase::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(health), NVP(material));
}

void BlockBase::onDbLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer& gt)
{
    mat = nullptr;
    if(const ObjectStatic *os = DB::Get(material))
    {
        if(const Material *o = os->GetAgent<Material>())
        {
            LOG(trace) << "material " << material << " linked";
            mat = const_cast<Material*>(o);
        }
    }
}

void BlockBase::onDamage(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(mat)
        mat->onDamage(par, l, pos, gt);
}

void BlockBase::onDestroy(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(mat)
        mat->onDestroy(par, l, pos, gt);
}

