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

    std::shared_ptr<Object> Instantiate(const glm::vec3 &pos, const GameTimer &gt);

    void   onDbLoad();
    void     onInit(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void   onUpdate(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void     onDraw(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void  onDestroy(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void   onDamage(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void    onEnter(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void    onLeave(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    void onInteract(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
};

#endif // OBJECTSTATIC_H
