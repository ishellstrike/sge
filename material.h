#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>
#include <string>
#include "texture.h"
#include <memory>
struct Material
{
    Material(void);
    std::string id;
    std::shared_ptr<Texture> texture, normal;
    glm::vec4 emission;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
    float index_of_refraction;
};	
#endif //MATERIAL_H
