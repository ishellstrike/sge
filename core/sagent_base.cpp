#include "sagent_base.h"

std::string SAgentBase::Name() const
{
    return "AgentBase";
}

uniqId SAgentBase::Type() const
{
    return UniqueId::getTypeId<SAgentBase>();
}
