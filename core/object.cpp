#include "object.h"
#include "random.h"
#include "textureatlas.h"
#include "objectstatic.h"

Object::Object(ObjectStatic *__base) : base(__base)
{  
}

std::unique_ptr<Object> Object::Instantiate()
{
    return nullptr;
}

bool Object::Equals(std::shared_ptr<Object> &o)
{
    if( o.get() == this )
        return true;

    if(o && agents)
    {
        for( const std::shared_ptr<Agent> &a : *agents )
        {
            Agent *a2 = o->GetAgent( a->id );
            if( !a2 || !a2->Equals( a.get() ) )
                return false;
        }
    }
    else
        return false;

    return true;
}

bool Object::isStatic()
{
    return agents == nullptr;
}

void Object::onInit(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onInit(o, l, pos, gt);

    base->onInit(o, l, pos, gt);
}

void Object::onUpdate(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onUpdate(o, l, pos, gt);

    base->onUpdate(o, l, pos, gt);
}

void Object::onDraw(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onDraw(o, l, pos, gt);

    base->onDraw(o, l, pos, gt);
}

void Object::onDestroy(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onDestroy(o, l, pos, gt);

    base->onDestroy(o, l, pos, gt);
}

void Object::onDamage(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onDamage(o, l, pos, gt);

    base->onDamage(o, l, pos, gt);
}

void Object::onEnter(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onEnter(o, l, pos, gt);

    base->onEnter(o, l, pos, gt);
}

void Object::onLeave(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onLeave(o, l, pos, gt);

    base->onLeave(o, l, pos, gt);
}

void Object::onInteract(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onInteract(o, l, pos, gt);

    base->onInteract(o, l, pos, gt);
}

void Object::onEvent(Event &e)
{
    if(agents)
        for(const auto &a : *agents)
            a->onEvent(*this, e);

    //base->onEvent(*this, e);
}

