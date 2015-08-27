/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SPACESYSTEM_H
#define SPACESYSTEM_H
#include <vector>
#include <memory>

class SpaceObject;

class SpaceSystem
{
public:
    SpaceSystem();
    std::vector<std::shared_ptr<SpaceObject>> system;
};

#endif // SPACESYSTEM_H
