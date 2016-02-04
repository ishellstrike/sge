/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef BLOCKBASE_H
#define BLOCKBASE_H
#include "core/agents/agent.h"
#include "material.h"

class BlockBase : public StaticAgent
{
public:
    SAGENT(BlockBase)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;
    void onDbLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer &gt) override;
    void onDamage(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt) override;
    void onDestroy(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt) override;

    int health = 1;
    std::string material;
    Material *mat;
};

REGISTER_AGENT(BlockBase)

#endif // BLOCKBASE_H
