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
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() override;
    bool IsStatic() override;
};

REGISTER_AGENT(Entity)

#endif // ENTITY_H
