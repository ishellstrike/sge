/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef QUADPLANE_H
#define QUADPLANE_H
#include <memory>
#include "geometry/umesh.h"
#include "camera.h"
#include "sphereparamstorage.h"
#include "geometry/vpnt.h"

typedef VertPosUvUv VertexType;

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
        READY,
        PRE_READY,
        TEXTURE_READY,
        TEXTURE_PRE_READY
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

    std::shared_ptr< UMesh<VertexType> > terminal_mesh;
    glm::mat4 transformation; // матрица, поворачивающие каждлую из 6 граней квадрата в соответствующую сторону
    glm::vec3 subsurface_centers[4]; //центры виртуальных поддеревьев
    Status status = ERROR;
    int level = 0; //уровень поддерева
    glm::vec2 offset = {0,0}; /*< [0, 1] отствуп участка дерева от верхнего левого угла корня дерева*/
    float scale = 1; /*< [0, 1] масштабирование участка квад дерева, относительно всего дерева */

    bool is_terminal() const;
    void Render(const Camera &cam,
                int side,
                SphereParamsStorage *parent);
    void Update(const Camera &camera, float Rs, float eps, int max_divide, SphereParamsStorage *parent);
    void GenerateSubTexture(std::shared_ptr<Material> &t, SphereParamsStorage *parent, float devider = 1);
     std::shared_ptr<Material> sub_texture;
};

#endif // QUADPLANE_H
