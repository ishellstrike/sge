#include "quad.h"
#include "vpnt.h"

std::shared_ptr<Mesh> Quad::getMesh()
{
    if(sm_mesh != nullptr){
        return std::make_shared<Mesh>(*sm_mesh);
    }

    sm_mesh = std::make_shared<Mesh>();

    sm_mesh->Vertices.push_back(VertPosNormTanBiTex({-0.5, 0, -0.5}, {0,0}));
    sm_mesh->Vertices.push_back(VertPosNormTanBiTex({0.5,  0, -0.5}, {1,0}));
    sm_mesh->Vertices.push_back(VertPosNormTanBiTex({-0.5, 0,  0.5}, {0,1}));
    sm_mesh->Vertices.push_back(VertPosNormTanBiTex({0.5,  0,  0.5}, {1,1}));

    sm_mesh->Indices.push_back(0);
    sm_mesh->Indices.push_back(1);
    sm_mesh->Indices.push_back(2);

    sm_mesh->Indices.push_back(1);
    sm_mesh->Indices.push_back(3);
    sm_mesh->Indices.push_back(2);

    sm_mesh->Unindex();
    sm_mesh->computeNormal();

    return std::make_shared<Mesh>(*sm_mesh);
}

std::shared_ptr<Mesh> Quad::sm_mesh;
