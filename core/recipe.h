/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey
        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef RECIPE_H
#define RECIPE_H
#include <string>
#include <vector>

class Chest;

class RecipePart
{
public:
    enum Type
    {
      Item     = 0,
      Quality  = 1,
      Material = 2,
      Tool     = 3
    };
    
    std::string name;
    int level = -1;
    int count = -1;
};

class Recipe
{
public:
    typedef std::vector<RecipePart> AlterateParts; 
    std::vector<AlterateParts> recipe;
    
    void Deserialize(const rapidjson::Value &val);
    void IsReady(const Chest &chest) const;
    void Craft(const Chest &chest) const;
};

#endif //RECIPE_H
