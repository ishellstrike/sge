#ifndef AGGRESSIVE_H
#define AGGRESSIVE_H
#include "core/agent.h"

class Creature;
class Sound;

class Aggressive : public StaticAgent
{
public:
    SAGENT(Aggressive)

    void Deserialize(const rapidjson::Value &val) override;
    virtual void onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    virtual void onInit(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    virtual void onLoad(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    Creature *owner;
    Sound *agro;
    std::string sound;
    std::vector<std::string> allies;
};

REGISTER_AGENT(Aggressive)

#endif // AGGRESSIVE_H
