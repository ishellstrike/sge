/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SOUND_H
#define SOUND_H

#ifdef CLIENT
#define C_BAKUP
#undef CLIENT
#endif

#include "core/agents/agent.h"
#ifdef CLIENT
#include "remsnd.h"
#endif

class SgeSound : public StaticAgent
{
public:
    SAGENT(SgeSound)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;

#ifdef CLIENT
    virtual void onDbLoad(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    virtual void onUpdate(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
#endif
    void Play() const;

    std::string name, filename;
    int near; int far = 8;
#ifdef CLIENT
    mutable RemSnd snd;
#endif
    void Move(const glm::vec3 &p) const;
    void Pitch(float p) const;

};

REGISTER_AGENT(SgeSound)

#ifdef C_BAKUP
#define CLIENT
#endif

#endif // SOUND_H
