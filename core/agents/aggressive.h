/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef AGGRESSIVE_H
#define AGGRESSIVE_H
#include "core/agents/agent.h"

class Creature;
class Sound;

class Aggressive : public StaticAgent
{
public:
    SAGENT(Aggressive)

    void Deserialize(const rapidjson::Value &val) override;
    virtual void onUpdate(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    virtual void onInit(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    virtual void onDbLoad(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    Creature *owner;
    Sound *agro;
    std::string sound;
    std::vector<std::string> allies;
};

REGISTER_AGENT(Aggressive)

#endif // AGGRESSIVE_H
