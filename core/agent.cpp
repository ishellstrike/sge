#include "agent.h"

Agent::Agent()
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
