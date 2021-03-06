/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H

#include <map>
#include <memory>
#include <functional>
#include <boost/noncopyable.hpp>
#include "logger.h"

using namespace boost::log::trivial;

template <class IdType, class Base>
class ObjectFactory : boost::noncopyable
{
public:
    typedef IdType IdTypeUsing;
protected:
    typedef std::shared_ptr<Base> BasePtr;
    typedef std::function<BasePtr()> CreateFunc;
    typedef std::map<IdType, CreateFunc> FactoryMap;

public:
    BasePtr Create(const IdType & id) const
    {
        typename FactoryMap::const_iterator it = map_.find(id);
        return (it != map_.end()) ? (it->second)() : BasePtr();
    }

    void Add(const IdType & id, CreateFunc func)
    {
        auto i = map_.find( id );
        if ( i == map_.end() )
        {
            LOG(trace) << "agent class id = \"" << id << "\" register";
            map_.insert(  FactoryMap::value_type(id, func) );
        }
        else
        {
            LOG(error) << "agent class id = \"" << id << "\" override";
            i->second = func;
        }
    }

private:
    FactoryMap map_;
};

template <class T>
class RegisterElement
{
public:
    typedef std::shared_ptr<T> TPtr;
public:
    template <class Factory>
    RegisterElement(Factory & factory, const typename Factory::IdTypeUsing & id)
    {
        if (class_registered_++ == 0) // Jerry Schwarz counter
            factory.Add(id, &CreateElmImpl);
    }
private:
    static TPtr CreateElmImpl() { return TPtr( new T() ); }
    static int class_registered_;
};

template<class T> int RegisterElement<T>::class_registered_ = 0;

#endif // AGENTFACTORY_H
