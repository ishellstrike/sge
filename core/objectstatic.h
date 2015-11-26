#ifndef OBJECTSTATIC_H
#define OBJECTSTATIC_H
#include "agent.h"
#include "core_const.h"
#include <vector>
#include <memory>
#include "object.h"

class ObjectStatic : public ObjectHelper, public ObjectStaticHelper
{
public:
    ObjectStatic(const std::string & __id);

    bool isStatic();
    bool is_static = false;

    static std::unique_ptr<ObjectStatic> air;
    std::unique_ptr<Object> Instantiate();
};

#endif // OBJECTSTATIC_H
