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
    int otex = 0;

    bool isStatic();

    void onInit();
    void onUpdate();
    void onDraw();
    void onDestroy();
};

#endif // OBJECT_H
