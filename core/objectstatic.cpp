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
        for(const auto &i : *agents)
            if(!i->IsStatic())
                o->agents->push_back(i->Instantiate());

    return std::unique_ptr<Object>(o);
}
