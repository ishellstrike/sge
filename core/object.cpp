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

