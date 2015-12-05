#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include "core/agent.h"
#include "core/object.h"

class Entity : public Agent
{
public:
    AGENT(Entity)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val);
    std::unique_ptr<Agent> Instantiate();
    bool IsStatic() override;
};

REGISTER(Entity)

#endif // ENTITY_H
