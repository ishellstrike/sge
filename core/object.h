#ifndef OBJECT_H
#define OBJECT_H
#include "objecthelper.h"
#include <functional>

class ObjectStatic;
class Event;

class Object : public ObjectBase
{
public:
    Object(ObjectStatic *base);
    std::unique_ptr<Object> Instantiate();

    ObjectStatic *base;
    int otex = 0;

    bool Equals( std::shared_ptr<Object> &o );

    bool  isStatic();

    void     onInit(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void   onUpdate(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void     onDraw(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt);

#ifdef CLIENT
    void onInteract(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer& gt);
#endif

    void    onEvent(Event &e);

    void  onDestroy(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void   onDamage(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void    onEnter(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void    onLeave(std::shared_ptr<Object> &o, Level *l, const glm::vec3 &pos, const GameTimer &gt);

};

template<typename _Ty>
inline _Ty* agent_cast( Object &ag )
{
    if(agents)
    {
        auto tid = Agent::TidFor<T>()
        for(const auto &a : *agents)
        {
            Agent &agent = *a;
            if(agent.id == tid)
                return static_cast<T*>(&agent);
        }
    }
    return nullptr;
}

template<typename _Ty>
inline _Ty* agent_base_cast( Object &ag )
{

    if(base->agents)
    {
        auto tid = Agent::TidFor<T>()
        for(const auto &a : *base->agents)
        {
            Agent &agent = *a;
            if(agent.id == tid)
                return static_cast<T*>(&agent);
        }
    }
    return nullptr;
}

#endif // OBJECT_H
