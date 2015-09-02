#ifndef RING_H
#define RING_H
#include "umesh.h"
#include <memory>
#include <cmath>
#include <glm/glm.hpp>

template<typename _Ty>
std::shared_ptr<UMesh<_Ty>> GenerateRing(float r, float w, int sector, bool doubleside = true)
{
    auto m = std::make_shared<UMesh<_Ty>>();
    float t = 0, pre_t = -glm::two_pi<float>()/(float)(sector);
    int c = 0;
    for(int i = 1; i < sector + 1; ++i)
    {
        _Ty vert1,vert2,vert3,vert4;

        vert1.position = {r*sin(t), r*cos(t), 0};
        vert1.uv = {0,0};

        vert2.position = {(r-w)*sin(t), (r-w)*cos(t), 0};
        vert2.uv = {1,0};

        vert3.position = {(r-w)*sin(pre_t), (r-w)*cos(pre_t), 0};
        vert3.uv = {1,1};

        vert4.position = {r*sin(pre_t), r*cos(pre_t), 0};
        vert4.uv = {0,1};

        m->vertices.push_back(vert1);
        m->vertices.push_back(vert2);
        m->vertices.push_back(vert3);
        m->vertices.push_back(vert4);

        m->indices.push_back(c);
        m->indices.push_back(c+1);
        m->indices.push_back(c+2);
        m->indices.push_back(c);
        m->indices.push_back(c+2);
        m->indices.push_back(c+3);

        if( doubleside )
        {
            m->indices.push_back(c);
            m->indices.push_back(c+2);
            m->indices.push_back(c+1);
            m->indices.push_back(c);
            m->indices.push_back(c+3);
            m->indices.push_back(c+2);
        }

        c += 4;
        pre_t = t;
        t = i * glm::two_pi<float>()/(float)sector;
    }

    return m;
}

#endif // RING_H

