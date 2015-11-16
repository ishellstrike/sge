#ifndef UMATERIAL_H
#define UMATERIAL_H
#include <unordered_map>
#include "texture.h"
#include <memory>

class material_uniform
{
    virtual void Bind() const
    {

    }
};

class texture_uniform : public material_uniform
{
    void Bind() const override
    {

    }
};

class color_uniform : public material_uniform
{
    void Bind() const override
    {

    }
};

class umaterial
{
public:
    umaterial();

    std::unordered_map<std::string, std::shared_ptr<material_uniform>> uniforms;

    void SetUniform( const std::string &, std::shared_ptr<Texture> & )
    {
        //auto a = std::make_shared<texture_uniform>();
       // uniforms[name] = u;
    }
};

#endif // UMATERIAL_H
