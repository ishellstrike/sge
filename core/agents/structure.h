/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "core/agent.h"

struct StructuralPart
{
    Id id;
    int count;

    void Deserialize(const rapidjson::Value &val);
};

class Structure : public DynamicAgent
{
public:
    DAGENT(Structure)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;

    //not affect to equals (for correct item stacking)
    virtual bool Equals( Agent *o ) override;

    int count = 1;
    std::vector<StructuralPart> parts;
};

REGISTER_AGENT(Structure)

#endif // STRUCTURE_H
