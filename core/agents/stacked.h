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

    //not affect to equals (for correct item stacking)
    virtual bool Equals( Agent *o ) override;

    int count = 1;
};

REGISTER_AGENT(Stacked)

#endif // STACKED_H
