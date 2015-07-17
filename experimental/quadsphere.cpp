#include "quadsphere.h"
#include "geometry/icosahedron.h"
#include "glm/gtx/transform.hpp"

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
}

void QuadSphere::Render(const Camera &cam)
{
    for(int i = 0; i < 6; i++)
    {
        plane[i]->Render(cam, mat, basic, i);
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

