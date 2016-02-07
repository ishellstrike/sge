/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef CHEST_H
#define CHEST_H
#include <vector>
#include "core/object.h"

class Chest : public DynamicAgent
{
public:
    DAGENT(Chest)

    std::vector<std::shared_ptr<Object>> items;

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;

#ifdef CLIENT
    void onInteract(std::shared_ptr<Object> &par, Level *, const glm::vec3 &, const GameTimer&) override;
#endif

    void Combine();
};

REGISTER_AGENT(Chest)

#endif // CHEST_H
