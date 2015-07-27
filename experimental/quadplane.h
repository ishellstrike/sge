#ifndef QUADPLANE_H
#define QUADPLANE_H
#include <memory>
#include "geometry/mesh.h"
#include "camera.h"
#include "sphereparamstorage.h"

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

    std::shared_ptr<Mesh> terminal_mesh;
    glm::mat4 transformation;
    glm::vec3 subsurface_centers[4];
    Status status = ERROR;
    int level = 0;
    glm::vec2 offset = {0,0}; /*< [0,1] */
    float scale = 1; /*< [0, 1] */

    bool is_terminal() const;
    void Render(const Camera &cam,
                int side,
                SphereParamsStorage *parent);
    void Update(Camera &camera, float Rs, float eps, int max_divide, SphereParamsStorage *parent);
    std::vector<QuadPlane *> getRoute();
    void GenerateSubTexture(std::shared_ptr<Material> &t, SphereParamsStorage *parent);
};

#endif // QUADPLANE_H
