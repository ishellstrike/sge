#ifndef STARFIELD_H
#define STARFIELD_H
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "geometry/umesh.h"

struct VertPosCol
{
    glm::vec4 col;
    glm::vec3 pos;

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
