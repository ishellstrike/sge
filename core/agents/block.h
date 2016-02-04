/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef BLOCK_H
#define BLOCK_H
#include "core/agents/agent.h"

class Block : public DynamicAgent
{
public:
    DAGENT(Block)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;

    int health = 1;
};

REGISTER_AGENT(Block)

#endif // BLOCK_H
