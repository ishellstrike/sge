/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef STARFIELD_H
#define STARFIELD_H
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "..\geometry/umesh.h"

struct VertPosCol
{
    glm::vec3 pos;
    glm::vec4 col;

    static VertexInfo info;
};

class Starfield
{
public:
    Starfield();
    UMesh<VertPosCol> field;
    void Render(const Camera &cam);
};

#endif // STARFIELD_H
