#ifndef Item_H
#define Item_H
#include "core/agent.h"

class Item : public DynamicAgent
{
public:   
    DAGENT(Item)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;

    //not affect to equals (for correct item stacking)
    virtual bool Equals( Agent *o ) override;

    int count = 1;
};

REGISTER_AGENT(Item)

#endif // Item_H
