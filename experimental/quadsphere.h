#ifndef QUADSPHERE_H
#define QUADSPHERE_H
#include "quadplane.h"

class QuadSphere
{
public:
    QuadSphere();
    std::shared_ptr<QuadPlane> plane[6];
    void Render(const glm::mat4 &MVP);
    std::shared_ptr<BasicJargShader> basic;
    std::shared_ptr<Material> mat;
};

#endif // QUADSPHERE_H
