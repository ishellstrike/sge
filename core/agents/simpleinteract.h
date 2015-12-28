#ifndef SIMPLEINTERACT_H
#define SIMPLEINTERACT_H
#include "core/agent.h"
#include "core/agents/sound.h"

class SimpleInteract : public StaticAgent
{
public:
    SAGENT(SimpleInteract)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;

    void onInteract(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt) override;
    void onLoad(Object *par, Level *, const glm::vec3 &pos, const GameTimer &gt) override;

    std::string afterid, sound;
    const Sound *activate;

};

REGISTER_AGENT(SimpleInteract)

#endif // SIMPLEINTERACT_H
