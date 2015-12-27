#ifndef SOUND_H
#define SOUND_H
#include "core/agent.h"
#include "remsnd.h"

class Sound : public StaticAgent
{
public:
    SAGENT(Sound)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    virtual void onLoad(Object *par, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    virtual void onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void Play() const;

    std::string name, filename;
    int near = 1, far = 8;
    mutable RemSnd snd;
    void Move(const glm::vec3 &p) const;
};

REGISTER_AGENT(Sound)

#endif // SOUND_H
