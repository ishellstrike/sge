#ifndef SIMPLEINTERACT_H
#define SIMPLEINTERACT_H
#include "core/agent.h"

class SimpleInteract : public StaticAgent
{
public:
    SAGENT(SimpleInteract)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;

    std::string afterid;
};

REGISTER_AGENT(SimpleInteract)

#endif // SIMPLEINTERACT_H
