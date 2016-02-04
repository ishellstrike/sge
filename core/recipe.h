/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey
        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef RECIPE_H
#define RECIPE_H
#include <string>
#include <vector>
#include <map>
#include "rapidjson/document.h"

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
    
    std::string value;
    int count = -1;
    Type type;

    void Deserialize(const rapidjson::Value &val);

private:
    static std::map<std::string, Type> type_caster;
};

class ResultPart
{
public:
    std::string id;
    int count = -1;

    void Deserialize(const rapidjson::Value &val);
};

class Recipe
{
public:
    typedef std::vector<RecipePart> AlterateParts; 
    std::vector<AlterateParts> recipe;
    std::vector<ResultPart> result;
    std::string name, descr, subcategory, category;
    
    void Deserialize(const rapidjson::Value &val);
    void IsReady(const Chest &chest) const;
    void Craft(const Chest &chest) const;
};

#endif //RECIPE_H
