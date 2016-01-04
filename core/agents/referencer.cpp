#include "referencer.h"
#include "core/db.h"

void Referencer::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(ref));
}

void Referencer::onLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &, const GameTimer &)
{
    const ObjectStatic *os = DB::Get("some");
    if(!os)
        LOG(error) << "broken reference";
    else
    {
        LOG(trace) << "reference " << ref << " ok";
        for(auto &a : *os->agents)
        {
            par->PushAgent(a->Instantiate());
        }
    }
}
