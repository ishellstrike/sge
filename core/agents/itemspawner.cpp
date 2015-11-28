#include "itemspawner.h"
#include "logger.h"

void ItemSpawner::Deserialize(rapidjson::Value &val)
{
    for(decltype(val.Size()) i=0; i < val.Size(); i++)
    {
        rapidjson::Value &p = val[i];

        SpawnInfo si;

        si.id            = p.Begin()->GetString();
        si.low_quantity  = p[1].GetInt();
        si.high_quantity = p[2].GetInt();
        si.probability   = p[3].GetDouble();

        items.push_back(si);
    }
}

std::unique_ptr<Agent> ItemSpawner::Instantiate()
{
    ItemSpawner *i = new ItemSpawner();
    return std::unique_ptr<ItemSpawner>(i);
}
