#ifndef CHEST_H
#define CHEST_H
#include <vector>
#include "core/agent.h"
#include "core/object.h"

class Chest : public Agent
{
public:
    AGENT(Chest)

    std::vector<std::shared_ptr<Object>> items;

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() override;
    bool IsStatic() override;
};

REGISTER_AGENT(Chest)

#endif // CHEST_H
