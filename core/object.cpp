#include "object.h"
#include "random.h"
#include "textureatlas.h"

Object::Object(ObjectStaticHelper *__base) : base(__base)
{  
}

std::unique_ptr<Object> Object::Instantiate()
{
    return nullptr;
}

bool Object::isStatic()
{
    return agents == nullptr;
}

void Object::onInit()
{
    if(agents)
        for(const auto &a : *agents)
            a->onInit(this);
}

void Object::onUpdate()
{
    if(agents)
        for(const auto &a : *agents)
            a->onUpdate(this);
}

void Object::onDraw()
{
    if(agents)
        for(const auto &a : *agents)
            a->onDraw(this);
}

void Object::onDestroy()
{
    if(agents)
        for(const auto &a : *agents)
            a->onDestroy(this);
}

