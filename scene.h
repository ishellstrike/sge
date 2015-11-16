#ifndef SCENE_H
#define SCENE_H
#include <list>
#include "light.h"

#include <core/sobject.hpp>

class Scene
{
public:
    Scene();

    std::list<SObject> meshes;
    std::list<Light> lights;
    Camera camera;

    void Render(const Camera &cam);
};

#endif // SCENE_H
