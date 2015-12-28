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

class ItemSpawner : public StaticAgent
{
public:
    SAGENT(ItemSpawner)

    std::vector<std::vector<SpawnInfo>> items;

    // Agent interface
public:

    /*!
    *\brief Create items in Chest agents while mapgen
    *
    *   spawn group format
    *   [
    *       [<string id>, <int low_quantity>, <int high_quantity>, <double probability>], //if first itemm pool does'n spawned
    *       [<string id>, <int low_quantity>, <int high_quantity>, <double probability>], //moving to next
    *       ...
    *   ]
    *
    *   Example:
    *   {
    *       "type":"ItemSpawner",
    *       "items":[
    *           [["foo_block", 1, 10, 0.4], ["bar_block", 1, 10, 0.6]] //spawn group chance 100%
    *       ]
    *   }
    */
    void Deserialize(const rapidjson::Value &val) override;

    void onInit(Object *par, Level *, const glm::vec3 &pos, const GameTimer &gt) override;
};

REGISTER_AGENT(ItemSpawner)

#endif // ITEMSPAWNER_H
