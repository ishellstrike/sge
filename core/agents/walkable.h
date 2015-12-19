#ifndef WALKABLE_H
#define WALKABLE_H
#include "core/agent.h"

class Walkable : public Agent
{
public:
    AGENT(Walkable)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() override;

    bool IsStatic() override;
};

REGISTER_AGENT(Walkable)

#endif // WALKABLE_H
