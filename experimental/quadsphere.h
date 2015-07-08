#ifndef QUADSPHERE_H
#define QUADSPHERE_H
#include "quadplane.h"
#include "camera.h"

class QuadSphere
{
public:
    QuadSphere();
    std::shared_ptr<QuadPlane> plane[6];
    void Render(const glm::mat4 &MVP);
    std::shared_ptr<BasicJargShader> basic;
    std::shared_ptr<Material> mat;

    float R = 1000, /*< Радиус сферы*/
          s = 20;  /*< Коэффициент деформации сферы*/

    void Update(Camera &camera);
    std::string out;
};

#endif // QUADSPHERE_H
