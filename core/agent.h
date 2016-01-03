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
#include <boost/noncopyable.hpp>
#include "gametimer.h"

class ObjectBase;
using namespace boost::signals2;

#define DAGENT(type)                     \
type() :                                 \
    DynamicAgent(Agent::TidFor<type>())  \
{                                        \
}                                        \
virtual ~type(){}                        \
type(const type&) = delete;              \
type& operator=(const type&) = delete;   \
virtual std::string Typename() { return #type; }

#define SAGENT(type)                     \
type() :                                 \
    StaticAgent(Agent::TidFor<type>())   \
{                                        \
}                                        \
virtual ~type(){}                        \
type(const type&) = delete;              \
type& operator=(const type&) = delete;   \
virtual std::string Typename() { return #type; }

#define REGISTER_AGENT(ctype)                                               \
namespace                                                                   \
{                                                                           \
RegisterElement<ctype> RegisterElement##ctype(AgentFactory::instance(), #ctype); \
}

class Object;
class Level;
class Win;

class Agent
{
    friend class Object;
    friend class ObjectBase;
    Tid id;

    static Tid NextTid()
    {
        static int next_id(0);
        return next_id++;
    }

public:
    Agent(int __id);

    template <typename T_>
    static Tid TidFor()
    {
        static int result(NextTid());
        return result;
    }

    virtual bool IsStatic() const = 0;
    virtual bool Equals( Agent *o );

    Tid GetTid();
    virtual std::string Typename() = 0;

    virtual void Deserialize(const rapidjson::Value &val);
    virtual std::shared_ptr<Agent> Instantiate() const = 0;

    virtual void     onLoad(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    virtual void     onInit(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    virtual void   onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    virtual void     onDraw(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    virtual void onInteract(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    virtual void   onDamage(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    virtual void  onDestroy(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    virtual void    onEnter(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    virtual void    onLeave(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
};

class DynamicAgent : public Agent
{
public:
    DynamicAgent(int __id) : Agent(__id) {}
    virtual bool IsStatic() const override final;
};

class StaticAgent : public Agent
{
public:
    StaticAgent(int __id) : Agent(__id) {}
    virtual bool IsStatic() const override final;
    virtual std::shared_ptr<Agent> Instantiate() const override final;
};

struct AgentFactory : public boost::noncopyable
{
  static ObjectFactory<std::string, Agent> &instance()
  {
      typedef ObjectFactory<std::string, Agent> OfType;
      static auto af = std::unique_ptr<OfType>( new OfType() );

      return *af;
  }
};

#endif // AGENT_H
