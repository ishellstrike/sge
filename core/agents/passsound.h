#ifndef PASSSOUND_H
#define PASSSOUND_H
#include "core/agent.h"
#include "sound.h"

class PassSound : public StaticAgent
{
public:
    SAGENT(PassSound)

    // Agent interface
public:

    /**
    *\brief Cause object make sound while somebody enter it.
    * Sound selection based on agility and difficulty, if incoming has it, otherwise only on random.
    *
    *   sound format:
    *   [<less than 50% of required agillity>,<50-99%>,<enough agility>],
    *
    *   Example:
    *   {
    *           "type":"PassSound",
    *           "sound":["sound_crackle","sound_rustle","sound_none"],
    *           "difficulty":5
    *   }
    */
    void Deserialize(rapidjson::Value &val) override;
    void onEnter(ObjectHelper *par, const glm::vec3 &pos, const GameTimer &gt) override;

    std::string ref;
    Sound *low, *middle, *pass;
};

REGISTER_AGENT(PassSound)

#endif // PASSSOUND_H
