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
    void    onInit(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void  onUpdate(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void    onDraw(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void onDestroy(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void  onDamage(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void   onEnter(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void   onLeave(Object *o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
};

#endif // OBJECTSTATIC_H
