/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "walkable.h"

void Walkable::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(cost));
}
