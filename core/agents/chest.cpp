#include "chest.h"




void Chest::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Chest::Instantiate()
{
    Chest *c = new Chest();
    return std::unique_ptr<Chest>(c);
}

bool Chest::IsStatic()
{
    return false;
}
