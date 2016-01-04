#include "simpleinteract.h"
#include "core/objectstatic.h"
#include "core/db.h"
#include "core/level.h"

void SimpleInteract::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(afterid), NVP(sound));
}

void SimpleInteract::onInteract(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(activate)
    {
        activate->Move(pos);
        activate->Play();
    }
    l->SetBlockAtPos(pos, DB::Create(afterid));
}

void SimpleInteract::onLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer& gt)
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
