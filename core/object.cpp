#include "object.h"

Object::Object(ObjectStaticHelper *__base) : base(__base)
{  
}

std::unique_ptr<Object> Object::Instantiate()
{
    Object *o = new Object(base);

    if(o->agents)
        for(const auto &i : *agents)
            o->agents->push_back(i->Instantiate());

    return std::unique_ptr<Object>(o);
}

bool Object::isStatic()
{
    return agents == nullptr;
}

