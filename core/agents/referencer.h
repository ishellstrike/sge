/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef REFERENCER_H
#define REFERENCER_H
#include "core/agent.h"

class Referencer : public StaticAgent
{
public:
    SAGENT(Referencer)

    // Agent interface
public:

    /**
    *\brief Copy all agents from ref object
    *
    *   Example:
    *   {
    *       "ref":<string id>
    *   }
    */
    void Deserialize(const rapidjson::Value &val) override;
    void onDbLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer& gt) override;

    std::string ref;
};

REGISTER_AGENT(Referencer)

#endif // REFERENCER_H
