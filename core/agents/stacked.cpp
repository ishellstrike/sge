#include "stacked.h"


void Stacked::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Stacked::Instantiate()
{
    Stacked *s = new Stacked();

    return std::unique_ptr<Stacked>(s);
}

bool Stacked::IsStatic()
{
    return false;
}
