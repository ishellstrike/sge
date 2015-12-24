#ifndef OBJECTSTATIC_H
#define OBJECTSTATIC_H
#include "agent.h"
#include "core_const.h"
#include <vector>
#include <memory>
#include "object.h"

class ObjectStatic : public ObjectBase
{
public:
    ObjectStatic(const std::string & __id);

    Id id;
    std::vector<Tex> tex;

    std::unique_ptr<Object> Instantiate(const glm::vec3 &pos, const GameTimer &gt);

    void    onLoad();
    void    onInit(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt);
    void  onUpdate(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt);
    void    onDraw(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt);
    void onDestroy(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt);
    void   onEnter(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt);
    void   onLeave(ObjectBase *o, const glm::vec3 &pos, const GameTimer &gt);
};

#endif // OBJECTSTATIC_H
