#include "stacked.h"


void Item::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Item::Instantiate() const
{
    auto s = std::make_unique<Item>();
    s->count = count;
    return std::move(s);
}

bool Item::Equals(Agent *o)
{
    return true;
}

