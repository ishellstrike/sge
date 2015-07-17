#ifndef QUADPLANE_H
#define QUADPLANE_H
#include <memory>
#include "geometry/mesh.h"
#include "camera.h"

class QuadPlane
{    enum Neighbours {
        TOP_N,
        BOTTOM_N,
        LEFT_N,
        RIGHT_N,
        NEIGHB_COUNT
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
     enum PARTS {
         TR,
         TL,
         DR,
         DL
     };

    QuadPlane();
    std::shared_ptr<QuadPlane> m_parts[4];
    QuadPlane *parent = nullptr;
    void getRoute(QuadPlane *from, std::vector<PARTS> &path, Neighbours that_neib);

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
    void Render(const Camera &cam, std::shared_ptr<Material> &mat, std::shared_ptr<BasicJargShader> &basic, int side);
    void Update(Camera &camera, float Rs, float eps, int max_divide);
    std::vector<QuadPlane *> getRoute();
};

#endif // QUADPLANE_H
