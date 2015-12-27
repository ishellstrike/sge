#ifndef TRANSPARENT_H
#define TRANSPARENT_H
#include "core/agent.h"

class Transparent : public StaticAgent
{
public:
    SAGENT(Transparent)
};

REGISTER_AGENT(Transparent)

#endif // TRANSPARENT_H
