#ifndef SOUND_H
#define SOUND_H
#include "core/agent.h"

class Sound : public StaticAgent
{
public:
    SAGENT(Sound)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;

    std::string ref;
};

REGISTER_AGENT(Sound)

#endif // SOUND_H
