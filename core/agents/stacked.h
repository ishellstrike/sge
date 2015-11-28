#ifndef STACKED_H
#define STACKED_H
#include "core/agent.h"

class Stacked : public Agent
{
public:   
    AGENT(Stacked)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val);
    std::unique_ptr<Agent> Instantiate();
};

#endif // STACKED_H
