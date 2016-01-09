/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef TAGS_H
#define TAGS_H
#include "core/agent.h"

class Tags : public StaticAgent
{
public:
    SAGENT(Tags)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;
    std::vector<std::string> tags;
};

REGISTER_AGENT(Tags)

#endif // TAGS_H
