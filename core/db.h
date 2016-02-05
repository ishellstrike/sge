/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef DB_H
#define DB_H
#include "objectstatic.h"
#include <unordered_map>
#include <memory>
#include "scheme.h"
#include "recipe.h"

class SgeSound;

class DB
{
public:
    DB();
    static std::unordered_map<std::string, std::unique_ptr<ObjectStatic>> data;
    static std::unordered_map<std::string, std::vector<ObjectStatic *>> tags_ref;
    static std::list<SgeSound *> sounds;

    static std::unordered_map<SchemeType, std::vector<Scheme>> scheme_db;
    static std::list<std::shared_ptr<Recipe>> recipe_db;
    static std::shared_ptr<Object> Create(const std::string &id);
    static const ObjectStatic *Get(const std::string &id);

    static void Load();
};

#endif // DB_H
