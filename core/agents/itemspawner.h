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

    std::vector<SpawnInfo> items;

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val);
    std::unique_ptr<Agent> Instantiate();

    void onInit(ObjectHelper *par) override;
};

REGISTER(ItemSpawner)

#endif // ITEMSPAWNER_H
