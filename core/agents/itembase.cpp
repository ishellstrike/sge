#include "itembase.h"

void ItemBase::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(volume, weight, sort);
}

std::shared_ptr<Agent> ItemBase::Instantiate()
{
    return std::unique_ptr<ItemBase>(new ItemBase());
}

bool ItemBase::IsStatic()
{
    return true;
}
