#include "objectstatic.h"
#include "textureatlas.h"

ObjectStatic::ObjectStatic(const std::string &__id)
{
    id = __id;
}

std::unique_ptr<Object> ObjectStatic::Instantiate(const glm::vec3 &pos, const GameTimer& gt)
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
        o->onInit(nullptr, pos, gt); // внутри вызывается так же обновление всех статических агентов
    }



    return std::unique_ptr<Object>(o);
}

void ObjectStatic::onLoad()
{
    if(agents)
        for(const auto &a : *agents)
                a->onLoad(nullptr, nullptr, {0,0,0}, GameTimer());
}

void ObjectStatic::onInit(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onInit(o, l, pos, gt);
}

void ObjectStatic::onUpdate(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onUpdate(o, l, pos, gt);
}

void ObjectStatic::onDraw(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onDraw(o, l, pos, gt);
}

void ObjectStatic::onDestroy(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onDestroy(o, l, pos, gt);
}

void ObjectStatic::onEnter(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onEnter(o, l, pos, gt);
}

void ObjectStatic::onLeave(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onLeave(o, l, pos, gt);
}
