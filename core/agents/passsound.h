/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef PASSSOUND_H
#define PASSSOUND_H
#include "core/agents/agent.h"
#include "sge_sound.h"

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
    void Deserialize(const rapidjson::Value &val) override;
    void onEnter(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer& gt) override;
    void onDbLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer& gt) override;

    std::string ref_l, ref_m, ref_p;
    const SgeSound *low, *middle, *pass;
};

REGISTER_AGENT(PassSound)

#endif // PASSSOUND_H
