#include "simpleinteract.h"

void SimpleInteract::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(afterid));
}
