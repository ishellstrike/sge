#ifndef REFERENCER_H
#define REFERENCER_H
#include "core/agent.h"

class Referencer : public StaticAgent
{
public:
    SAGENT(Referencer)

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
    void onLoad(Object *par, Level *, const glm::vec3 &pos, const GameTimer& gt) override;

    std::string ref;
};

REGISTER_AGENT(Referencer)

#endif // REFERENCER_H
