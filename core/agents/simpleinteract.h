/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SIMPLEINTERACT_H
#define SIMPLEINTERACT_H
#include "core/agents/agent.h"
#include "core/agents/sge_sound.h"

class SimpleInteract : public StaticAgent
{
public:
    SAGENT(SimpleInteract)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;

#ifdef CLIENT
    void onInteract(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt) override;
#endif

    void onDbLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer &gt) override;

    std::string afterid, sound;
    const SgeSound *activate;

};

REGISTER_AGENT(SimpleInteract)

#endif // SIMPLEINTERACT_H
