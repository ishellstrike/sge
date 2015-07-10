#include "quadsphere.h"
#include "geometry/icosahedron.h"
#include "glm/gtx/transform.hpp"

QuadSphere::QuadSphere()
{
    basic = std::make_shared<BasicJargShader>();
    basic->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal.glsl");
    basic->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal.glsl");
    basic->Link();
    basic->Use();
    basic->Afterlink();

    mat = std::make_shared<Material>();
    std::shared_ptr<Texture> texx = std::make_shared<Texture>();
    texx->Load("data/derevo.png", true, true);
    std::shared_ptr<Texture> texxx = std::make_shared<Texture>();
    texxx->Load("data/aaa.png", true, true);
    mat->texture = texx;
    mat->normal = texxx;

    for(int i = 0; i < 6; i++)
    {
        plane[i] = std::make_shared<QuadPlane>();
        plane[i]->terminal_mesh = std::make_shared<Mesh>();
    }


    plane[1]->transformation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0,1,0));
    plane[2]->transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(0,1,0));
    plane[3]->transformation = glm::rotate(glm::mat4(1), -glm::half_pi<float>(), glm::vec3(0,1,0));
    plane[4]->transformation = glm::rotate(glm::mat4(1), -glm::half_pi<float>(), glm::vec3(1,0,0));
    plane[5]->transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1,0,0));
}

void QuadSphere::Render(const glm::mat4 &MVP)
{
    for(int i = 0; i < 6; i++)
    {
        plane[i]->Render(MVP, mat, basic, i);
    }
}

void QuadSphere::Update(Camera &camera)
{
    std::stringstream ss;
    for(int i = 0; i < 6; i++)
    {
        ss << std::to_string(glm::distance(plane[i]->subsurface_centers[0] * R, camera.getPosition()))
                 << " " << std::to_string(glm::distance(plane[i]->subsurface_centers[1] * R, camera.getPosition()))
                 << " " << std::to_string(glm::distance(plane[i]->subsurface_centers[2] * R, camera.getPosition()))
                 << " " << std::to_string(glm::distance(plane[i]->subsurface_centers[3] * R, camera.getPosition())) << std::endl;

        plane[i]->Update(camera, R + s, R * 2);
    }
    out = ss.str();
}

