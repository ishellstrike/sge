/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef Creature_H
#define Creature_H
#include <vector>
#include "core/agents/agent.h"
#include "core/object.h"

class Order
{
public:
    enum Type {
        Nothing,
        Wander,
        Walk,
        Attack,
        Follow,
        Delay
    };

    Order() {}
    Order(const glm::vec3 &p, Type t) : pos(p), type(t) {}

    glm::vec3 pos;
    std::weak_ptr<Object> target; //stores therefore must be weak_ptr
    Type type = Nothing;
    float time;
};

class Creature : public DynamicAgent
{
public:
    DAGENT(Creature)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;
    virtual void onUpdate(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt);

    glm::vec3 pos;
    Order current_order;
};

REGISTER_AGENT(Creature)

#endif // Creature_H
