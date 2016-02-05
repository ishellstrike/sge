#include "objectstatic.h"
#include "textureatlas.h"

ObjectStatic::ObjectStatic(const std::string &__id)
{
    id = __id;
}

std::shared_ptr<Object> ObjectStatic::Instantiate(const glm::vec3 &pos, const GameTimer& gt)
{
    std::shared_ptr<Object> o = std::make_shared<Object>(this);

    if(tex.size() > 0)
        o->otex = random::next() * tex.size();

    if(agents)
    {
        o->agents = std::unique_ptr<AgentContainer>(new AgentContainer());
        for(const auto &i : *agents)
        {
            if( !i->IsStatic() )
                o->agents->push_back(i->Instantiate());
        }
        o->onInit(o, nullptr, pos, gt); // внутри вызывается так же обновление всех статических агентов
    }



    return o;
}

void ObjectStatic::onDbLoad()
{
    if(agents)
        for(const auto &a : *agents)
                a->onDbLoad(std::shared_ptr<Object>(), nullptr, {0,0,0}, GameTimer());
}

void ObjectStatic::onInit(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onInit(o, l, pos, gt);
}

void ObjectStatic::onUpdate(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onUpdate(o, l, pos, gt);
}

void ObjectStatic::onDraw(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onDraw(o, l, pos, gt);
}

void ObjectStatic::onDestroy(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onDestroy(o, l, pos, gt);
}

void ObjectStatic::onEnter(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onEnter(o, l, pos, gt);
}

void ObjectStatic::onDamage(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onDamage(o, l, pos, gt);
}

void ObjectStatic::onLeave(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onLeave(o, l, pos, gt);
}

#ifdef CLIENT
void ObjectStatic::onInteract(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            if(a->IsStatic())
                a->onInteract(o, l, pos, gt);
}
#endif
