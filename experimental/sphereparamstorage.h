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
    std::vector<GLuint> Indeces;
    GLuint ibo;
    glm::mat4 world;

    float R = 1000, /*< Радиус сферы*/
          s = 100;  /*< Коэффициент деформации сферы*/

    int minimum = 0;
    int max_divide = 8;
    int tess_size = 32;
    glm::vec3 center = {0,0,0};
};

#endif // SPHEREPARAMSTORAGE

