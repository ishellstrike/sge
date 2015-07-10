#ifndef QUADPLANE_H
#define QUADPLANE_H
#include <memory>
#include "geometry/mesh.h"
#include "camera.h"

class QuadPlane
{
    enum {
        NW_PLANE,
        NE_PLANE,
        SW_PLANE,
        SE_PLANE
    };

    enum Type {
        Same,
        Lower
    };

    enum Status {
        ERROR,
        READY
    };
public:
    QuadPlane();
    std::shared_ptr<QuadPlane> m_parts[4];
    Type nighbours[4];

    std::shared_ptr<QuadPlane> &NW();
    std::shared_ptr<QuadPlane> &NE();
    std::shared_ptr<QuadPlane> &SW();
    std::shared_ptr<QuadPlane> &SE();

    std::shared_ptr<Mesh> terminal_mesh;
    glm::mat4 transformation;
    glm::vec3 subsurface_centers[4];
    Status status = ERROR;
    int level = 0;
    glm::vec2 offset = {0,0};
    float scale = 1;

    bool is_terminal() const;
    void Render(const glm::mat4 &MVP, std::shared_ptr<Material> &mat, std::shared_ptr<BasicJargShader> &basic, int side);
    void Update(Camera &camera, float Rs, float eps);
};

#endif // QUADPLANE_H
