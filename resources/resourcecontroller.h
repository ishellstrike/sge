﻿/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef RESOURCECONTROLLER_H
#define RESOURCECONTROLLER_H
#include <unordered_map>
#include "logger.h"
#include <mutex>
#include "resources/texture.h"
#include "basicjargshader.h"

#define _STRINGIFY(t) #t
#define STRINGIFY(t) _STRINGIFY(t)
#define PUSH_NVP(a) Push(STRINGIFY(a), a)

template<typename _Ty>
class ResourceDictionary
{
public:
    std::shared_ptr<_Ty> &operator[](const std::string &id)
    {
        const auto & find = data.find(id);
        if(find == data.end())
            LOG(fatal) << typeid(_Ty).name() << " id = \"" << id << "\" missed";
        return find->second;
    }

    void Push(const std::string &name, _Ty *val)
    {
        data.insert(std::make_pair(name, std::shared_ptr<_Ty>(val)));
    }

private:
    std::unordered_map<std::string, std::shared_ptr<_Ty>> data;

};

class Resources
{
public:
    static Resources *instance()
    {
        static std::mutex mutex;
        if(!m_inst)
        {
            mutex.lock();

            if (!m_inst)
            {
                LOG(trace) << "making resources controller instance";
                m_inst = new Resources();
            }

            mutex.unlock();
        }
        return m_inst;
    }

    static void drop()
    {
        static std::mutex mutex;
        mutex.lock();
        if(m_inst)
        {
            delete m_inst;
            m_inst = nullptr;
        }
        mutex.unlock();
        LOG(trace) << "droping resources controller instance";
    }

    void Init();

    template<typename _Ty>
    void Push(const std::string &name,_Ty *res)
    {
        static_assert(0, "not implemented");
    }
    template<>
    void Push(const std::string &name, BasicJargShader *res)
    {
        LOG(trace) << "pushing \"" << name << "\" BasicJargShader resource";
        shaders.Push(name, res);
    }template<>
    void Push(const std::string &name, Shader *res)
    {
        LOG(trace) << "pushing \"" << name << "\" Shader resource";
        u_shaders.Push(name, res);
    }
    template<>
    void Push(const std::string &name, Texture *res)
    {
        LOG(trace) << "pushing \"" << name << "\" Texture resource";
        textures.Push(name, res);
    }

    template<typename _Ty>
    void Push(std::pair<std::string, _Ty> val)
    {
        Push(val.first, val.second);
    }

    template<typename _Ty>
    std::shared_ptr<_Ty>& Get(const std::string &name)
    {
        static_assert(0, "not implemented");
    }
    template<>
    std::shared_ptr<Shader>& Get(const std::string &name)
    {
        return u_shaders[name];
    }
    template<>
    std::shared_ptr<BasicJargShader>& Get(const std::string &name)
    {
        return shaders[name];
    }
    template<>
    std::shared_ptr<Texture>& Get(const std::string &name)
    {
        return textures[name];
    }

private:
    static Resources *m_inst;
    Resources(){}
    ~Resources(){}
    Resources(const Resources &root) = delete;
    Resources &operator=(const Resources &) = delete;

    ResourceDictionary<BasicJargShader> shaders;
    ResourceDictionary<Shader> u_shaders;
    ResourceDictionary<Texture> textures;
};

#endif // RESOURCECONTROLLER_H
