#include "itembase.h"

void ItemBase::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(volume), NVP(weight), NVP(sort), NVP(description), NVP(name));
}
