/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "creaturebase.h"

void CreatureBase::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(name), NVP(description));
}
