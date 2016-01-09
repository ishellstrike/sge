/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "stacked.h"


void Item::Deserialize(const rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Item::Instantiate() const
{
    auto s = std::make_unique<Item>();
    s->count = count;
    return std::move(s);
}

bool Item::Equals(Agent *o)
{
    return true;
}

