#ifndef CHEST_H
#define CHEST_H
#include <vector>
#include "core/agent.h"
#include "core/object.h"

class Chest : public Agent
{
public:
    AGENT(Chest)

    std::vector<Object> items;

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val);
    std::unique_ptr<Agent> Instantiate();
};

#endif // CHEST_H
