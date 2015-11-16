#ifndef SAGENT_H
#define SAGENT_H
#include "uniqueid.h"
#include <glm/glm.hpp>
#include "helper.h"

#define _STR(s) #s
#define STR(s) _STR(s)

#define AGENT(name) \
    std::string Name() const override \
    {                                 \
        return STR(name);             \
    }                                 \
    uniqId Type() const override      \
    {                                 \
        return UniqueId::getTypeId<name>(); \
    }

class SAgentBase
{
public:

   virtual std::string Name() const;
   virtual uniqId Type() const;
};

#endif // SAGENT_H
