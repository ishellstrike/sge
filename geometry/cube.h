#ifndef CUBE_H
#define CUBE_H
#include "umesh.h"
#include <memory>
#include <cmath>
#include <glm/glm.hpp>

namespace
{
glm::vec3 cube_vertices[] = {
    {-0.5, -0.5,  0.5},
    { 0.5, -0.5,  0.5},
    { 0.5,  0.5,  0.5},
    {-0.5,  0.5,  0.5},
    {-0.5, -0.5, -0.5},
    { 0.5, -0.5, -0.5},
    { 0.5,  0.5, -0.5},
    {-0.5,  0.5, -0.5},
  };
}

template<typename _Ty>
std::shared_ptr<UMesh<_Ty>> GenerateCube()
{
    std::shared_ptr<UMesh<_Ty>> m = std::make_shared<UMesh<_Ty>>();

    m->indices = {
        0, 1, 2, 2, 3, 0, 3, 2, 6, 6, 7, 3,
        7, 6, 5, 5, 4, 7, 4, 5, 1, 1, 0, 4,
        4, 0, 3, 3, 7, 4, 1, 5, 6, 6, 2, 1
      };

    for(auto &a : cube_vertices)
    {
        _Ty p;
        p.position = a;
        m->vertices.push_back(p);
    }

    return m;
}

#endif // CUBE_H

