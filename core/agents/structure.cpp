/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "structure.h"



void Structure::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(parts));
}

std::shared_ptr<Agent> Structure::Instantiate() const
{
    Structure *s = new Structure;
    return std::shared_ptr<Agent>(s);
}

bool Structure::Equals(Agent *o)
{
    return false;
}


void StructuralPart::Deserialize(const rapidjson::Value &val)
{
    id = val.Begin()->GetString();
    count = val[1].GetInt();
}
