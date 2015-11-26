#ifndef OBJECT_H
#define OBJECT_H
#include "objecthelper.h"
#include <functional>

class Object : public ObjectHelper
{
public:
    Object(ObjectStaticHelper *base);
    std::unique_ptr<Object> Instantiate();

    ObjectStaticHelper *base;

    bool isStatic();
};

#endif // OBJECT_H
