#ifndef ITEMSPAWNER_H
#define ITEMSPAWNER_H
#include "core/agent.h"
#include <vector>

struct SpawnInfo
{
    Id id;
    int low_quantity = 1, high_quantity = 1;
    float probability = 1.0f;
};

class ItemSpawner : public Agent
{
public:
    AGENT(ItemSpawner)

    std::vector<std::vector<SpawnInfo>> items;

    // Agent interface
public:

    /*!
    *\brief Агент, осуществляющий генерацию предметов при создании блоков-контейнеров
    *
    *   Example:
    *   {
    *       "type":"ItemSpawner",
    *       "items":[
    *           [[<string id>, <int low_quantity>, <int high_quantity>, <double probability>]],
    *           [["foo_block", 1, 10, 0.4]]
    *       ]
    *   }
    */
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() override;

    void onInit(ObjectHelper *par) override;

};

REGISTER_AGENT(ItemSpawner)

#endif // ITEMSPAWNER_H
