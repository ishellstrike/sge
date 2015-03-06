#pragma once
#ifndef SphereTesselator_h__
#define SphereTesselator_h__

#include "mesh.h"
class Tesselator
{
public:
    static Mesh *Tesselate(int iters, Mesh &mesh);
    static void SphereTesselate(int iters, std::shared_ptr<Mesh> &mesh);
private:
    static Mesh* SubTesselate(const Mesh& mesh);
    static void SphereSubTesselate(std::shared_ptr<Mesh> &mesh);
};
#endif // SphereTesselator_h__

