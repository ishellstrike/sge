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

bool Object::Equals(Object *o)
{
    if( o == this )
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

void Object::onInit(Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onInit(this, l, pos, gt);

    base->onInit(this, l, pos, gt);
}

void Object::onUpdate(Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onUpdate(this, l, pos, gt);

    base->onUpdate(this, l, pos, gt);
}

void Object::onDraw(Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onDraw(this, l, pos, gt);

    base->onDraw(this, l, pos, gt);
}

void Object::onDestroy(Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onDestroy(this, l, pos, gt);

    base->onDestroy(this, l, pos, gt);
}

void Object::onDamage(Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onDamage(this, l, pos, gt);

    base->onDamage(this, l, pos, gt);
}

void Object::onEnter(Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onEnter(this, l, pos, gt);

    base->onEnter(this, l, pos, gt);
}

void Object::onLeave(Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onLeave(this, l, pos, gt);

    base->onLeave(this, l, pos, gt);
}

