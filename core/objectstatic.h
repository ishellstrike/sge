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

    std::unique_ptr<Object> Instantiate(const glm::vec3 &pos, const GameTimer &gt);

    void    onInit(ObjectHelper *o, const glm::vec3 &pos, const GameTimer &gt);
    void  onUpdate(ObjectHelper *o, const glm::vec3 &pos, const GameTimer &gt);
    void    onDraw(ObjectHelper *o, const glm::vec3 &pos, const GameTimer &gt);
    void onDestroy(ObjectHelper *o, const glm::vec3 &pos, const GameTimer &gt);
    void   onEnter(ObjectHelper *o, const glm::vec3 &pos, const GameTimer &gt);
    void   onLeave(ObjectHelper *o, const glm::vec3 &pos, const GameTimer &gt);
};

#endif // OBJECTSTATIC_H
