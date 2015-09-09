#ifndef MESHBASE_H
#define MESHBASE_H
#include <glm/glm.hpp>
#include "camera.h"

class MeshBase
{
public:
    MeshBase();

    virtual void Bind() = 0;
    virtual void ForgetBind() = 0;
    virtual void Render(const Camera &cam, const glm::mat4 &world = glm::mat4(1)) = 0;

    template<typename _Ty, typename T>
    void ComputeAABB(const T _Ty::*field) = 0;
};

#endif // MESHBASE_H
