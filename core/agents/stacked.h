#ifndef STACKED_H
#define STACKED_H
#include "core/agent.h"

class Stacked : public Agent
{
public:   
    AGENT(Stacked)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() override;

    int count;
    bool IsStatic() override;
};

REGISTER_AGENT(Stacked)

#endif // STACKED_H
