/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef AGENT_HPPP
#define AGENT_HPPP
#include <core/core_const.h>
#include <memory>
#include <string>
#include <boost/noncopyable.hpp>

#include "boost/signals2.hpp"
#include "agentfactory.h"
#include "core/serialize.h"
#include "gametimer.h"

class Event;
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
    virtual ~Agent();

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

    virtual void   onDbLoad(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    virtual void     onInit(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    virtual void   onUpdate(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    virtual void     onDraw(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    virtual void onInteract(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    virtual void    onEvent(Object &par, Event &e);

    virtual void   onDamage(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    virtual void  onDestroy(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);

    virtual void    onEnter(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
    virtual void    onLeave(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt);
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

#endif // AGENT_HPPP
