#ifndef STACKED_H
#define STACKED_H
#include "core/agent.h"

class Stacked : public DynamicAgent
{
public:   
    DAGENT(Stacked)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;

    int count;
};

REGISTER_AGENT(Stacked)

#endif // STACKED_H
