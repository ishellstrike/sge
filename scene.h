#ifndef SCENE_H
#define SCENE_H
#include <list>
#include "light.h"
#include "geometry/meshbase.h"

class Scene
{
public:
    Scene();

    std::list<MeshBase> meshes;
    std::list<Light> lights;
    Camera camera;

    void Render(const Camera &cam);
};

#endif // SCENE_H
