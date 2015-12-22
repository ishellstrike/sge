#ifndef OBJECT_H
#define OBJECT_H
#include "objecthelper.h"
#include <functional>

class ObjectStatic;

class Object : public ObjectHelper
{
public:
    Object(ObjectStatic *base);
    std::unique_ptr<Object> Instantiate();

    ObjectStatic *base;
    int otex = 0;

    bool  isStatic();

    void    onInit(const glm::vec3 &pos, const GameTimer &gt);
    void  onUpdate(const glm::vec3 &pos, const GameTimer &gt);
    void    onDraw(const glm::vec3 &pos, const GameTimer &gt);
    void onDestroy(const glm::vec3 &pos, const GameTimer &gt);
    void   onEnter(const glm::vec3 &pos, const GameTimer &gt);
    void   onLeave(const glm::vec3 &pos, const GameTimer &gt);
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
