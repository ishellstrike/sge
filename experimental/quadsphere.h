#ifndef QUADSPHERE_H
#define QUADSPHERE_H
#include "quadplane.h"
#include "camera.h"
#include "sphereparamstorage.h"

class QuadSphere : public SphereParamsStorage
{
public:
    QuadSphere(std::shared_ptr<BasicJargShader> &shader, std::shared_ptr<Material> &__mat);
    std::shared_ptr<QuadPlane> plane[6];

    int minimum = 0;

    float R = 1000, /*< Радиус сферы*/
          s = 100;  /*< Коэффициент деформации сферы*/

    int max_divide = 8;

    glm::vec3 center = {0,0,0};

    void Render(const Camera &cam);
    void Update(Camera &camera);
    std::string out;
};

#endif // QUADSPHERE_H
