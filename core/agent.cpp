#include "agent.h"

Agent::Agent()
{

}

Agent::Agent(int __id) :
    id(__id)
{

}

bool Agent::IsStatic()
{
    return true;
}

Tid Agent::GetTid()
{
    return id;
}

void Agent::onLoad(ObjectHelper *)
{

}

void Agent::onInit(ObjectHelper *)
{

}

void Agent::onUpdate(ObjectHelper *)
{

}

void Agent::onDraw(ObjectHelper *)
{

}

void Agent::onDestroy(ObjectHelper *)
{

}