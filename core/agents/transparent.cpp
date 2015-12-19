#include "transparent.h"



void Transparent::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Transparent::Instantiate()
{
    return std::make_shared<Transparent>();
}

bool Transparent::IsStatic()
{
    return true;
}
