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
    return is_static;
}

Tid Agent::GetTid()
{
    return id;
}
