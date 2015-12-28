#include "creaturebase.h"

void CreatureBase::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(name), NVP(description));
}
