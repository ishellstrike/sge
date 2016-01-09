/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "block.h"

void Block::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(health));
}

std::shared_ptr<Agent> Block::Instantiate() const
{
    auto b = std::make_shared<Block>();
    b->health = health;
    return b;
}
