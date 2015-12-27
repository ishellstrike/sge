#ifndef OBJECT_H
#define OBJECT_H
#include "objecthelper.h"
#include <functional>

class ObjectStatic;

class Object : public ObjectBase
{
public:
    Object(ObjectStatic *base);
    std::unique_ptr<Object> Instantiate();

    ObjectStatic *base;
    int otex = 0;

    bool Equals( Object *o );

    bool  isStatic();

    void    onInit(Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void  onUpdate(Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void    onDraw(Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void onDestroy(Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void   onEnter(Level *l, const glm::vec3 &pos, const GameTimer &gt);
    void   onLeave(Level *l, const glm::vec3 &pos, const GameTimer &gt);
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
