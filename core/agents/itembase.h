/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <vector>
#include "core/agents/agent.h"
#include "core/object.h"

class ItemBase : public StaticAgent
{
public:
    SAGENT(ItemBase)

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

    int volume, weight;
    std::string sort;
    std::string name, description;
};

REGISTER_AGENT(ItemBase)

#endif // ITEMBASE_H
