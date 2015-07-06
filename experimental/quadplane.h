#ifndef QUADPLANE_H
#define QUADPLANE_H
#include <memory>
#include "geometry/mesh.h"

class QuadPlane
{
    enum {
        NW_PLANE,
        NE_PLANE,
        SW_PLANE,
        SE_PLANE
    };

    enum Status {
        ERROR,
        READY
    };
public:
    QuadPlane();
    std::shared_ptr<QuadPlane> m_parts[4];

    std::shared_ptr<QuadPlane> &NW();
    std::shared_ptr<QuadPlane> &NE();
    std::shared_ptr<QuadPlane> &SW();
    std::shared_ptr<QuadPlane> &SE();

    std::shared_ptr<Mesh> terminal_mesh;
    Status status = ERROR;

    bool is_terminal() const;
    void Render(const glm::mat4 &MVP, std::shared_ptr<Material> &mat, std::shared_ptr<BasicJargShader> &basic);
};

#endif // QUADPLANE_H
