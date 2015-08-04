#ifndef QUADSPHERE_H
#define QUADSPHERE_H
#include "quadplane.h"
#include "camera.h"
#include "sphereparamstorage.h"

class QuadSphere : public SphereParamsStorage
{
public:
    QuadSphere(std::shared_ptr<Material> &__mat);
    ~QuadSphere();
    std::shared_ptr<QuadPlane> plane[6];

    void Render(const Camera &cam);
    void Update(const Camera &camera);
    std::string out;
};

#endif // QUADSPHERE_H
