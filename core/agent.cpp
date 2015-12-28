#include "agent.h"

Agent::Agent(int __id) :
    id(__id)
{

}

bool Agent::Equals(Agent *o)
{
    if( o == this )
        return true;
    if(o)
    {
        return o->id == id;
    }
    return false;
}

Tid Agent::GetTid()
{
    return id;
}

void Agent::Deserialize(const rapidjson::Value &val)
{

}

void Agent::onLoad(Object *, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onInit(Object *, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onUpdate(Object *, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onDraw(Object *, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onInteract(Object *, Level *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onDamage(Object *, Level *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onDestroy(Object *, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onEnter(Object *, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onLeave(Object *, Level *, const glm::vec3 &, const GameTimer&)
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
