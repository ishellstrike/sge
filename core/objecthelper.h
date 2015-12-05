#ifndef OBJECTHELPER_H
#define OBJECTHELPER_H
#include <memory>
#include <vector>
#include <functional>
#include "agent.h"

typedef std::vector<std::unique_ptr<Agent>> AgentContainer;

enum ObjectType
{
    ObjectError,
    ObjectItem,
    ObjectBlock,
    ObjectCreature,
    ObjectSpecial
};

class ObjectStaticHelper
{
public:
    Id id;
    std::vector<Tex> tex;
    std::string name = "air";
    ObjectType type;
    bool ground = false;
};

class ObjectHelper
{
public:
    std::unique_ptr<AgentContainer> agents;

    template <typename T>
    T *GetAgent()
    {
        for(const auto &a : *agents)
        {
            Agent &agent = *a;
            if(agent.id == Agent::TidFor<T>())
                return static_cast<T*>(&agent);
        }
        return nullptr;
    }

    template <typename T>
    bool HasAgent()
    {
        for(const auto &a : *agents)
        {
            Agent &agent = *a;
            if(agent.id == Agent::TidFor<T>())
                return true;
        }
        return false;
    }

    template <typename T>
    bool ForAgent(std::function<void(T*)> a)
    {
        T *agent = GetAgent<T>();
        if(agent)
        {
            a(agent->agent);
            return true;
        }
        return false;
    }

    template <typename T>
    void PushAgent(std::unique_ptr<T> agent)
    {
        agents->push_back(std::move(agent));
    }
};

#endif // OBJECTHELPER_H
