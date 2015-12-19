#ifndef SIMPLEINTERACT_H
#define SIMPLEINTERACT_H
#include "core/agent.h"

class SimpleInteract : public Agent
{
public:
    AGENT(SimpleInteract)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() override;

    bool IsStatic() override;

    std::string afterid;
};

REGISTER_AGENT(SimpleInteract)

#endif // SIMPLEINTERACT_H
