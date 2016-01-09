/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef WALKABLE_H
#define WALKABLE_H
#include "core/agent.h"

class Walkable : public StaticAgent
{
public:
    SAGENT(Walkable)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;

    float cost = 1;
};

REGISTER_AGENT(Walkable)

#endif // WALKABLE_H
