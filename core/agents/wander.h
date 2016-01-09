/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef WANDER_H
#define WANDER_H
#include "core/agent.h"

class Wander : public StaticAgent
{
public:
    SAGENT(Wander)

    virtual void onUpdate(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt);
};

REGISTER_AGENT(Wander)

#endif // WANDER_H
