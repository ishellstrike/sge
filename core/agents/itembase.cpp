/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "itembase.h"

void ItemBase::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(volume), NVP(weight), NVP(sort), NVP(description), NVP(name));
}
