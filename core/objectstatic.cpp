#include "objectstatic.h"
#include "textureatlas.h"

ObjectStatic::ObjectStatic(const std::string &__id)
{
    id = __id;
}

bool ObjectStatic::isStatic()
{
    return is_static;
}

std::unique_ptr<Object> ObjectStatic::Instantiate()
{
    Object *o = new Object(this);

    if(tex.size() > 0)
        o->otex = rand()%tex.size();

    if(is_static)
        return std::unique_ptr<Object>(o);

    if(agents)
    {
        o->agents = std::unique_ptr<AgentContainer>(new AgentContainer());
        for(const auto &i : *agents)
        {
            if( i->IsStatic() )
                o->agents->push_back( i );
            else
                o->agents->push_back(i->Instantiate());
        }
        o->onInit();
        std::remove_if(std::begin(*o->agents), std::end(*o->agents), [](const std::shared_ptr<Agent> &ag)
        {
            return ag->IsStatic();
        });
    }



    return std::unique_ptr<Object>(o);
}
