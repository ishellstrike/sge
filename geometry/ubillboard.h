#ifndef UBILLBOARD_H
#define UBILLBOARD_H
#include <geometry/vpnt.h>
#include <geometry/umesh.h>
#include "camera.h"

template <class _Ty = VertPosNormUvUv>
class UBillboard : public UMesh<_Ty>
{
public:
    UBillboard() : UMesh(){}
    virtual ~UBillboard(){}

    struct billboard_data {
        billboard_data(glm::vec2 s, glm::vec3 c) : size(s), center(c) {}
        glm::vec2 size;
        glm::vec3 center;
    };

    std::vector<billboard_data> billboards;

    void Prepare(const Camera &cam)
    {
        static const glm::vec3 vert[] = {{-0.5,-0.5,0},{0.5,-0.5,0},{0.5,0.5,0},{-0.5,0.5,0}};

        int j = 0;
        for(const auto &a : billboards)
        {
            for(int i = 0; i < 4; ++i)
            {
                vertices[j*4 + i].position = a.center + cam.Up() * vert[i].x * a.size.x + cam.Right() * vert[i].y * a.size.y;
            }
            ++j;
        }
    }
};

#endif // UBILLBOARD_H
