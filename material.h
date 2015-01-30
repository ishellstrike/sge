#pragma once
#include <GL/glew.h>
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



struct PointLight
{
    PointLight(void);
    glm::vec4 position;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    //////////////////////////////////////////////////////////////////////////
    // x - constant attenuation
    // y - linear attenuation
    // z - quadratic attenuation
    //////////////////////////////////////////////////////////////////////////
    glm::vec3 attenuation;
};

