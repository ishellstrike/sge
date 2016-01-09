/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

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
    void Deserialize(const rapidjson::Value &val) override;
    virtual void onDbLoad(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    virtual void onUpdate(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void Play() const;

    std::string name, filename;
    int near = 1, far = 8;
    mutable RemSnd snd;
    void Move(const glm::vec3 &p) const;
    void Pitch(float p) const;
};

REGISTER_AGENT(Sound)

#endif // SOUND_H
