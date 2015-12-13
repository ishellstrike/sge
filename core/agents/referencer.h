#ifndef REFERENCER_H
#define REFERENCER_H
#include "core/agent.h"

class Referencer : public Agent
{
public:
    AGENT(Referencer)

    // Agent interface
public:

    /**
    *\brief Copy all agents from ref object
    *
    *   Example:
    *   {
    *       "ref":<string id>
    *   }
    */
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() override;
    bool IsStatic() override;
    void onLoad(ObjectHelper *par) override;

    std::string ref;
};

REGISTER_AGENT(Referencer)

#endif // REFERENCER_H
