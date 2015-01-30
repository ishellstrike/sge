#include "material.h"

Material::Material(void) :
    emission(glm::vec4(0.0f, 0.0f, 0.0f, 1.f)),
    ambient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)),
    diffuse(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)),
    shininess(8.0f),
    specular(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)),
    index_of_refraction(0),
    texture(nullptr),
    normal(nullptr)
{
}


PointLight::PointLight(void) :
    ambient(0),
    diffuse(0),
    specular(0),
    position(0),
    attenuation(0)
{

}
