#include "referencer.h"

void Referencer::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(ref);
}

std::shared_ptr<Agent> Referencer::Instantiate()
{
    return std::unique_ptr<Referencer>(i);
}

bool Referencer::IsStatic()
{
    return true;
}

void Referencer::onLoad(ObjectHelper *par)
{
    ObjectStatic *os = DB::Get("some");
    for(auto &a : os->agents)
    {
        par->PushAgent(a.Instantoate());
    }
}
