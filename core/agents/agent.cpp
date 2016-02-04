#include "agent.h"

Agent::Agent(int __id) :
    id(__id)
{

}

Agent::~Agent()
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

void Agent::Deserialize(const rapidjson::Value &)
{

}

void Agent::onDbLoad(std::shared_ptr<Object> &, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onInit(std::shared_ptr<Object> &, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onUpdate(std::shared_ptr<Object> &, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onDraw(std::shared_ptr<Object> &, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onInteract(std::shared_ptr<Object> &, Level *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onEvent(Object &par, Event &e)
{

}

void Agent::onDamage(std::shared_ptr<Object> &, Level *, const glm::vec3 &, const GameTimer &)
{

}

void Agent::onDestroy(std::shared_ptr<Object> &, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onEnter(std::shared_ptr<Object> &, Level *, const glm::vec3 &, const GameTimer&)
{

}

void Agent::onLeave(std::shared_ptr<Object> &, Level *, const glm::vec3 &, const GameTimer&)
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
