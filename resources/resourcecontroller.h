#ifndef RESOURCECONTROLLER_H
#define RESOURCECONTROLLER_H
#include <unordered_map>
#include "logger.h"
#include <mutex>
#include "resources/material.h"
#include "resources/texture.h"
#include "basicjargshader.h"

template<typename _Ty>
class ResourceDictionary
{
public:
    std::shared_ptr<_Ty> &operator[](const std::string &id)
    {
        auto find = data.find(id);
        if(find == data.end())
            return data["error"];
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
                LOG(verbose) << "making resources controller instance";
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
        LOG(verbose) << "droping resources controller instance";
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
        LOG(verbose) << "pushing \"" << name << "\" BasicJargShader resource";
        shaders.Push(name, res);
    }
    template<>
    void Push(const std::string &name, Material *res)
    {
        LOG(verbose) << "pushing \"" << name << "\" Material resource";
        materials.Push(name, res);
    }
    template<>
    void Push(const std::string &name, Texture *res)
    {
        LOG(verbose) << "pushing \"" << name << "\" Texture resource";
        textures.Push(name, res);
    }

    template<typename _Ty>
    std::shared_ptr<_Ty>& Get(const std::string &name)
    {
        static_assert(0, "not implemented");
    }
    template<>
    std::shared_ptr<BasicJargShader>& Get(const std::string &name)
    {
        return shaders[name];
    }
    template<>
    std::shared_ptr<Material>& Get(const std::string &name)
    {
        return materials[name];
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
    ResourceDictionary<Material> materials;
    ResourceDictionary<Texture> textures;
};

#endif // RESOURCECONTROLLER_H
