#ifndef OBJECTHELPER_H
#define OBJECTHELPER_H
#include <memory>
#include <vector>
#include <functional>
#include "agent.h"

typedef std::vector<std::unique_ptr<Agent>> AgentContainer;


class ObjectStaticHelper
{
public:
    Id id;
    Tex tex;
    std::string name = "air";
};

class ObjectHelper
{
public:
    std::unique_ptr<AgentContainer> agents;

    template <typename T>
    T *GetAgent()
    {
        for(auto a : agents)
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
        for(auto a : agents)
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
    void PushAgent(std::shared_ptr<T> agent)
    {
        agents.push_back(agent);
    }
};

#endif // OBJECTHELPER_H
