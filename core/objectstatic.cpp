#include "objectstatic.h"
#include "textureatlas.h"

ObjectStatic::ObjectStatic(const std::string &__id)
{
    id = __id;
}

std::unique_ptr<Object> ObjectStatic::Instantiate(const glm::vec3 &pos, const GameTimer &gt)
{
    Object *o = new Object(this);

    if(tex.size() > 0)
        o->otex = rand()%tex.size();

    if(agents)
    {
        o->agents = std::unique_ptr<AgentContainer>(new AgentContainer());
        for(const auto &i : *agents)
        {
            if( !i->IsStatic() )
                o->agents->push_back(i->Instantiate());
        }
        o->onInit(pos, gt); // внутри вызывается так же обновление всех статических агентов
    }



    return std::unique_ptr<Object>(o);
}

void ObjectStatic::onInit(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onInit(o, pos, gt);
}

void ObjectStatic::onUpdate(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onUpdate(o, pos, gt);
}

void ObjectStatic::onDraw(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onDraw(o, pos, gt);
}

void ObjectStatic::onDestroy(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onDestroy(o, pos, gt);
}

void ObjectStatic::onEnter(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onEnter(o, pos, gt);
}

void ObjectStatic::onLeave(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onLeave(o, pos, gt);
}
