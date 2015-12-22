#include "stacked.h"


void Stacked::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Stacked::Instantiate() const
{
    return std::make_unique<Stacked>();
}

