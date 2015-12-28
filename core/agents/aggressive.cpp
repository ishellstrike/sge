#include "aggressive.h"
#include "core/objectstatic.h"
#include "core/db.h"
#include "core/agents/sound.h"
#include "core/agents/entity.h"
#include "core/level.h"

void Aggressive::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(sound), NVP(allies));
}

void Aggressive::onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
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
                if(!i.lock()->base->HasAgent<Wander>())
                {
                    owner->current_order.type = Order::Follow;
                    owner->current_order.target = i;
                    return;
                }
            }
        }
    }
}

void Aggressive::onInit(Object *par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    owner = par->GetAgent<Creature>();
    if(!owner)
    {
        LOG(error) << "non Creature object has Aggressive agent";
    }
}

void Aggressive::onLoad(Object *par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    agro = nullptr;
    if(const ObjectStatic *os = DB::Get(sound))
    {
        if(const Sound *o = os->GetAgent<Sound>())
        {
            LOG(trace) << "aggro sound " << sound << " linked";
            agro = const_cast<Sound*>(o);
        }
    }
}
