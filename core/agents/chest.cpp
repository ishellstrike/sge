#include "chest.h"




void Chest::Deserialize(rapidjson::Value &val)
{

}

std::unique_ptr<Agent> Chest::Instantiate()
{
    Chest *c = new Chest();
    return std::unique_ptr<Chest>(c);
}
