#include "itembase.h"

void ItemBase::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(volume), NVP(weight), NVP(sort), NVP(description), NVP(name));
}
