#ifndef TRANSPARENT_H
#define TRANSPARENT_H
#include "core/agent.h"

class Transparent : public Agent
{
public:
    AGENT(Transparent)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() override;

    bool IsStatic() override;
};

REGISTER_AGENT(Transparent)

#endif // TRANSPARENT_H
