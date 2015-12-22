#ifndef TRANSPARENT_H
#define TRANSPARENT_H
#include "core/agent.h"

class Transparent : public StaticAgent
{
public:
    SAGENT(Transparent)

    // Agent interface
public:

    void Deserialize(rapidjson::Value &val) override;
};

REGISTER_AGENT(Transparent)

#endif // TRANSPARENT_H
