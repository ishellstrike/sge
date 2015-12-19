#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <vector>
#include "core/agent.h"
#include "core/object.h"

class ItemBase : public Agent
{
public:
    AGENT(ItemBase)

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
    std::shared_ptr<Agent> Instantiate() override;
    bool IsStatic() override;

    int volume, weight;
    std::string sort;
    std::string name, description;
};

REGISTER_AGENT(ItemBase)

#endif // ITEMBASE_H
