#ifndef AGENT_H
#define AGENT_H
#include <core/core_const.h>
#include <memory>
#include "rapidjson/document.h"

#define AGENT(type)                         \
type() :                                    \
    Agent(#type, Agent::typeid_for<type>()) \
{                                           \
}                                           \
virtual ~type(){}                           \
type(const type&) = delete;                 \
type& operator=(const type&) = delete;

#define CASTER(ctype)                                                \
if(strcmp(part["type"].GetString(), #ctype) == 0)                    \
{                                                                    \
    std::unique_ptr<ctype> c = std::unique_ptr<ctype>(new ctype());  \
    c->Deserialize(part);                                            \
    b->PushAgent(c);                                                 \
    LOG(verbose) << #ctype << " added";                              \
} else

class Agent
{
    Tid id;
    bool is_static = true;

    static Tid NextTid()
    {
        static int next_id(0);
        return next_id++;
    }

public:
    Agent();

    static Tid TidFor()
    {
        static int result(NextTid());
        return result;
    }

    bool IsStatic();

    Tid GetTid();

    virtual void Deserialize(rapidjson::Value &val) = 0;
    virtual std::unique_ptr<Agent> Instantiate() = 0;
};

#endif // AGENT_H
