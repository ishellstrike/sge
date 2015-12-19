#include "simpleinteract.h"

void SimpleInteract::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(afterid));
}

std::shared_ptr<Agent> SimpleInteract::Instantiate()
{
    return std::make_shared<SimpleInteract>();
}

bool SimpleInteract::IsStatic()
{
    return false;
}
