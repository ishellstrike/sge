#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <vector>
#include "core/agent.h"
#include "core/object.h"

class ItemBase : public StaticAgent
{
public:
    SAGENT(ItemBase)

    // Agent interface
public:

    /**
    *\brief Basic item values
    *
    *   Example:
    *   {
    *       "weight":<int>,
    *       "volume":<int>,
    *       "sort":<string sort type>
    *   }
    */
    void Deserialize(rapidjson::Value &val) override;

    int volume, weight;
    std::string sort;
    std::string name, description;
};

REGISTER_AGENT(ItemBase)

#endif // ITEMBASE_H
