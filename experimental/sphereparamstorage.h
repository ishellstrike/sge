/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

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
    std::vector<GLuint> indices;
    GLuint ibo;
    glm::mat4 world;

    float R = 1000, /*< Радиус сферы*/
          s = 100;  /*< Коэффициент деформации сферы*/

    int minimum = 0;
    int max_divide = 10;
    int tess_size = 32;
    glm::vec3 center = {0,0,0};

    GLuint shader_r, shader_s;
};

#endif // SPHEREPARAMSTORAGE

