/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#pragma once
#ifndef VertexPositionTexture_h__
#define VertexPositionTexture_h__

#include "../colorextender.h"
#include <glm/glm.hpp>
#include "vertexinfo.h"

struct VertexPositionTexture{
    glm::vec3 position;
    glm::vec2 uv;
    VertexPositionTexture(glm::vec3 __v, glm::vec3 __p , glm::vec2 __u);
    VertexPositionTexture();
    ~VertexPositionTexture();
};

/**
 * @brief The VPNTBT struct. Vertex position normal tangent binormal texture
 */
struct VertPosNormUvUv{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec2 uv_glob;
    VertPosNormUvUv(glm::vec3 __norm, glm::vec3 __pos, glm::vec2 __uv);
    VertPosNormUvUv(glm::vec3 __norm, glm::vec3 __pos, glm::vec2 __uv, glm::vec2 __uv2);
    VertPosNormUvUv(glm::vec3 __pos, glm::vec2 __uv);
    VertPosNormUvUv();
    ~VertPosNormUvUv();

    static VertexInfo info;
};

/**
 * @brief The VPNTBT struct. Vertex position texture texture
 */
struct VertPosUvUv{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec2 uv_glob;
    VertPosUvUv(glm::vec3 __pos, glm::vec2 __uv);
    VertPosUvUv(glm::vec3 __pos, glm::vec2 __uv, glm::vec2 __uv2);
    VertPosUvUv();
    ~VertPosUvUv();

    static VertexInfo info;
};

/**
 * @brief The VPNTBT struct. Vertex position texture texture barycentric
 */
struct VertPosUvUvBa{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec2 uv_glob;
    glm::vec3 bary;
    VertPosUvUvBa(glm::vec3 __pos, glm::vec2 __uv, glm::vec2 __uv2, glm::vec3 __bary);
    VertPosUvUvBa();
    ~VertPosUvUvBa();

    static VertexInfo info;
};

/**
 * @brief The VPNTBT struct. Vertex position texture
 */
struct VertPosUv{
    glm::vec3 position;
    glm::vec2 uv;
    VertPosUv(glm::vec3 __pos, glm::vec2 __uv);
    VertPosUv();
    ~VertPosUv();

    static VertexInfo info;
};

/**
 * @brief The VPNTBT struct. Vertex position
 */
struct VertPos{
    glm::vec3 position;
    VertPos(glm::vec3 __pos);
    VertPos();
    ~VertPos();

    static VertexInfo info;
};

/**
 * @brief The VPNTBT struct. Vertex position bytenormal texture
 */
class VertPosMininormTex{
public:
    glm::vec3 position;
    unsigned char normal;
    glm::vec2 uv;
    VertPosMininormTex(unsigned char __norm, glm::vec3 __pos, glm::vec2 __uv);
    VertPosMininormTex(glm::vec3 __pos, glm::vec2 __uv);
    VertPosMininormTex();
    ~VertPosMininormTex();
};

class VertexPositionColor{
public:
    glm::vec3 pos;
    glm::vec4 col;
};
#endif // VertexPositionTexture_h__
