#pragma once
#ifndef AGENT_H
#define AGENT_H
#include <core/core_const.h>
#include <memory>
#include "rapidjson/document.h"
#include "boost/signals2.hpp"
#include "agents/agentfactory.h"
#include <string>
#include "core/serialize.h"

class ObjectHelper;
using namespace boost::signals2;

#define AGENT(type)                    \
type() :                               \
    Agent(Agent::TidFor<type>())       \
{                                      \
}                                      \
virtual ~type(){}                      \
type(const type&) = delete;            \
type& operator=(const type&) = delete; \
virtual std::string Typename() { return #type; }

#define REGISTER_AGENT(ctype)                                               \
namespace                                                                   \
{                                                                           \
RegisterElement<ctype> RegisterElement##ctype(AgentFactory::instance(), #ctype); \
}

class Agent
{
    friend class ObjectHelper;
    Tid id;

    static Tid NextTid()
    {
        static int next_id(0);
        return next_id++;
    }

public:
    Agent();
    Agent(int __id);

    template <typename T_>
    static Tid TidFor()
    {
        static int result(NextTid());
        return result;
    }

    virtual bool IsStatic();

    Tid GetTid();
    virtual std::string Typename() = 0;

    virtual void Deserialize(rapidjson::Value &val) = 0;
    virtual std::shared_ptr<Agent> Instantiate() = 0;

    virtual void onLoad(ObjectHelper *par);
    virtual void onInit(ObjectHelper *par);
    virtual void onUpdate(ObjectHelper *par);
    virtual void onDraw(ObjectHelper *par);
    virtual void onDestroy(ObjectHelper *par);
};

struct AgentFactory
{
  static ObjectFactory<std::string, Agent> &instance()
  {
      typedef ObjectFactory<std::string, Agent> OfType;
      static auto af = std::unique_ptr<OfType>( new OfType() );

      return *af;
  }
};

#endif // AGENT_H
