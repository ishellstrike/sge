#include "material.h"
#include "core/db.h"
#include "core/objectstatic.h"

void Material::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(hitsound), NVP(breaksound));
}

void Material::onLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer &gt)
{
    hitting = nullptr;
    if(const ObjectStatic *os = DB::Get(hitsound))
    {
        if(const Sound *o = os->GetAgent<Sound>())
        {
            LOG(trace) << "sound " << hitsound << " linked";
            hitting = o;
        }
    }

    breaking = nullptr;
    if(const ObjectStatic *os = DB::Get(breaksound))
    {
        if(const Sound *o = os->GetAgent<Sound>())
        {
            LOG(trace) << "sound " << breaksound << " linked";
            breaking = o;
        }
    }
}

void Material::onDamage(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(hitting)
    {
        hitting->Move(pos);
        hitting->Play();
    }
}

void Material::onDestroy(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(breaking)
    {
        breaking->Move(pos);
        breaking->Play();
    }
}
