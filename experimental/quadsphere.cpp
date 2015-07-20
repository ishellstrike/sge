#include "quadsphere.h"
#include "geometry/icosahedron.h"
#include "glm/gtx/transform.hpp"
#include "TextureGenerator.h"

QuadSphere::QuadSphere(std::shared_ptr<BasicJargShader> &shader, std::shared_ptr<Material> &__mat)
{
    basic = shader;
    mat = __mat;

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

    h_map = std::make_shared<BasicJargShader>();
    h_map->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/testgen1.glsl");
    h_map->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/testgen1.glsl");
    h_map->Link();
    h_map->Use();
    h_map->Afterlink();

    g_map = std::make_shared<BasicJargShader>();
    g_map->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/gradient_builder.glsl");
    g_map->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/gradient_builder.glsl");
    g_map->Link();
    g_map->Use();
    g_map->Afterlink();

    const float res = 2048.0f;
    TextureGenerator tg;
    std::shared_ptr<Texture> tnoise = std::make_shared<Texture>();
    std::shared_ptr<Texture> dnoise = std::make_shared<Texture>();
    std::shared_ptr<Texture> height_map = std::make_shared<Texture>(glm::vec2{res,res});
    tnoise->Load("data/PerlinPerm2D.png");
    dnoise->Load("data/PerlinGrad2D.png");
    tg.SetShader(h_map);
    tg.AddTexture("samplerPerlinPerm2D", tnoise);
    tg.AddTexture("samplerPerlinGrad2D", dnoise);
    tg.SetParams(0);
    tg.SetParams(0);
    tg.SetParams(1);
    tg.SetResultTexture(height_map);
    tg.RenderOnTempFbo();

    mat->global_height = height_map;
}

void QuadSphere::Render(const Camera &cam)
{
    for(int i = 0; i < 6; i++)
    {
        plane[i]->Render(cam, mat, basic, i, h_map, g_map);
    }
}

void QuadSphere::Update(Camera &camera)
{
    if(pow(center.x - camera.Position().x, 2) + pow(center.y - camera.Position().y, 2) + pow(center.z - camera.Position().z, 2) < R*R)
        return;
    for(int i = 0; i < 6; i++)
    {
        plane[i]->Update(camera, R + s, R * 2, max_divide);
    }
}

