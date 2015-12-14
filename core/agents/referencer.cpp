#include "referencer.h"
#include "core/db.h"

void Referencer::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(ref));
}

std::shared_ptr<Agent> Referencer::Instantiate()
{
    return std::make_shared<Referencer>();
}

bool Referencer::IsStatic()
{
    return true;
}

void Referencer::onLoad(ObjectHelper *par)
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
