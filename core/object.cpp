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

void Object::onInit(const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onInit(this, pos, gt);

    base->onInit(this, pos, gt);
}

void Object::onUpdate(const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onUpdate(this, pos, gt);

    base->onUpdate(this, pos, gt);
}

void Object::onDraw(const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onDraw(this, pos, gt);

    base->onDraw(this, pos, gt);
}

void Object::onDestroy(const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onDestroy(this, pos, gt);

    base->onDestroy(this, pos, gt);
}

void Object::onEnter(const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onEnter(this, pos, gt);

    base->onEnter(this, pos, gt);
}

void Object::onLeave(const glm::vec3 &pos, const GameTimer &gt)
{
    if(agents)
        for(const auto &a : *agents)
            a->onLeave(this, pos, gt);

    base->onLeave(this, pos, gt);
}

