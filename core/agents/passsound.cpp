/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "passsound.h"
#include "core/db.h"

void PassSound::Deserialize(const rapidjson::Value &val)
{
    if(val.HasMember("sound"))
    {
        const rapidjson::Value &arr = val["sound"];
        if(!arr.IsArray())
            throw;
        ref_l = arr.Begin()->GetString();
        ref_m = arr[1].GetString();
        ref_p = arr[2].GetString();
    }
}

void PassSound::onEnter(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer &gt)
{
    if(low)
    {
        low->Move(pos);
        low->Play();
    }
}

void PassSound::onDbLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer& gt)
{
    auto check = [&](const std::string &s)->const SgeSound*{
        if(const ObjectStatic *os = DB::Get(s))
        {
            if(const SgeSound *o = os->GetAgent<SgeSound>())
            {
                LOG(trace) << "pass sound " << s << " linked";
                return o;
            }
            return nullptr;
        }
    };

    low = check(ref_l);
    middle = check(ref_m);
    pass = check(ref_m);
}

