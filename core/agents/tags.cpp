/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "tags.h"



void Tags::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(tags));
}
