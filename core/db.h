#ifndef DB_H
#define DB_H
#include "objectstatic.h"
#include <unordered_map>
#include <memory>
#include "scheme.h"

class DB
{
public:
    DB();
    static std::unordered_map<std::string, std::unique_ptr<ObjectStatic>> data;
    static std::unordered_map<SchemeType, std::vector<Scheme>> scheme_db;
    static std::shared_ptr<Object> Create(const std::string &id);
    static const ObjectStatic *Get(const std::string &id);

    static void Load();
};

#endif // DB_H
