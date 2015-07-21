#ifndef SPHEREPARAMSTORAGE
#define SPHEREPARAMSTORAGE
#include "resources/texture.h"
#include "basicjargshader.h"
#include "resources/material.h"
#include <memory>

class SphereParamsStorage
{
public:
    std::shared_ptr<BasicJargShader> basic;
    std::shared_ptr<Material> mat;
    std::shared_ptr<BasicJargShader> height_shader, grad_shader;
    std::shared_ptr<Texture> noise_map, grad_map;
};

#endif // SPHEREPARAMSTORAGE

