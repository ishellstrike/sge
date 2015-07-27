#ifndef QUADSPHERE_H
#define QUADSPHERE_H
#include "quadplane.h"
#include "camera.h"
#include "sphereparamstorage.h"

class QuadSphere : public SphereParamsStorage
{
public:
    QuadSphere(std::shared_ptr<BasicJargShader> &shader, std::shared_ptr<Material> &__mat);
    ~QuadSphere();
    std::shared_ptr<QuadPlane> plane[6];

    void Render(const Camera &cam);
    void SetWorld(const glm::mat4 &world);
    void Update(Camera &camera);
    std::string out;
};

#endif // QUADSPHERE_H
