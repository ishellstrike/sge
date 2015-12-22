#include "agent.h"

Agent::Agent(int __id) :
    id(__id)
{

}

Tid Agent::GetTid()
{
    return id;
}

void Agent::onLoad(ObjectBase *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onInit(ObjectBase *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onUpdate(ObjectBase *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onDraw(ObjectBase *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onDestroy(ObjectBase *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onEnter(ObjectBase *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onLeave(ObjectBase *, const glm::vec3 &, const GameTimer &)
{

}


bool DynamicAgent::IsStatic() const
{
    return false;
}

bool StaticAgent::IsStatic() const
{
    return true;
}

std::shared_ptr<Agent> StaticAgent::Instantiate() const
{
    return nullptr;
}
