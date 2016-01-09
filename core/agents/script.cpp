/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "script.h"



void Script::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(source), NVP(path));
}
