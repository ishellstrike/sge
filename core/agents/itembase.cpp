#include "itembase.h"

void ItemBase::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(volume), NVP(weight), NVP(sort), NVP(description), NVP(name));
}

std::shared_ptr<Agent> ItemBase::Instantiate()
{
    return std::unique_ptr<ItemBase>(new ItemBase());
}

bool ItemBase::IsStatic()
{
    return true;
}
