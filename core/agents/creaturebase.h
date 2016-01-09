/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef CREATUREBASE_H
#define CREATUREBASE_H
#include <vector>
#include "core/agent.h"
#include "core/object.h"

class CreatureBase : public StaticAgent
{
public:
    SAGENT(CreatureBase)

    // Agent interface
public:

    /**
    *\brief Basic item values
    *
    *   Example:
    *   {
    *       "weight":<int>,
    *       "volume":<int>,
    *       "sort":<string sort type>
    *   }
    */
    void Deserialize(const rapidjson::Value &val) override;

    std::string name, description;
};

REGISTER_AGENT(CreatureBase)

#endif // CREATUREBASE_H
