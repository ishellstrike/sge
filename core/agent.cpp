#include "agent.h"

Agent::Agent(int __id) :
    id(__id)
{

}

Tid Agent::GetTid()
{
    return id;
}

void Agent::onLoad(ObjectHelper *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onInit(ObjectHelper *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onUpdate(ObjectHelper *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onDraw(ObjectHelper *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onDestroy(ObjectHelper *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onEnter(ObjectHelper *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onLeave(ObjectHelper *, const glm::vec3 &, const GameTimer &)
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
