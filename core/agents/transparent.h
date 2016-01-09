/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef TRANSPARENT_H
#define TRANSPARENT_H
#include "core/agent.h"

class Transparent : public StaticAgent
{
public:
    SAGENT(Transparent)
};

REGISTER_AGENT(Transparent)

#endif // TRANSPARENT_H
