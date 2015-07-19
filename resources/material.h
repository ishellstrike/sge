#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>
#include <string>
#include "texture.h"
#include <memory>
struct Material
{
    std::string id;
    std::shared_ptr<Texture> texture = nullptr, normal = nullptr, height = nullptr, grad = nullptr;
    glm::vec4 emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.f);
    glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    glm::vec4 specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    float shininess = 80.0f;
    float index_of_refraction = 1.33;
};	
#endif //MATERIAL_H
