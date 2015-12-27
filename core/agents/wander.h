#ifndef WANDER_H
#define WANDER_H
#include "core/agent.h"

class Wander : public StaticAgent
{
public:
    SAGENT(Wander)

    virtual void onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer &gt);
};

REGISTER_AGENT(Wander)

#endif // WANDER_H
