#pragma once
#ifndef VertexPositionTexture_h__
#define VertexPositionTexture_h__

#include "../colorextender.h"
#include <glm/glm.hpp>

enum BufferType
{
    BUFFER_TYPE_VERTEX,
    BUFFER_TYPE_TEXTCOORD,
    BUFFER_TYPE_NORMALE,
    BUFFER_TYPE_COLOR,
};

struct VertexPositionTexture{
public:
    glm::vec3 position;
    glm::vec2 uv;
    VertexPositionTexture(glm::vec3 __v, glm::vec3 __p , glm::vec2 __u);
    VertexPositionTexture();
    ~VertexPositionTexture();

    VertexPositionTexture operator + (VertexPositionTexture __a) const
    {
        VertexPositionTexture b;
        b.position = position + __a.position;
        b.uv = __a.uv + uv; return b;
    }
    VertexPositionTexture operator / (float __a) const
    {
        VertexPositionTexture b;
        b.position = position / __a;
        b.uv = uv / __a; return b;
    }
};

/**
 * @brief The VPNTBT struct. Vertex position normal tangent binormal texture
 */
struct VertPosNormUvUv{
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec2 uv_glob;
    VertPosNormUvUv(glm::vec3 __norm, glm::vec3 __pos, glm::vec2 __uv);
    VertPosNormUvUv(glm::vec3 __norm, glm::vec3 __pos, glm::vec2 __uv, glm::vec2 __uv2);
    VertPosNormUvUv(glm::vec3 __pos, glm::vec2 __uv);
    VertPosNormUvUv();
    ~VertPosNormUvUv();

    VertPosNormUvUv operator + (VertPosNormUvUv __a) const
    {
        VertPosNormUvUv b;
        b.position = position + __a.position;
        b.uv = __a.uv + uv;
        b.uv_glob = __a.uv_glob + uv_glob;
        b.normal = normal + __a.normal;
        return b;
    }

    VertPosNormUvUv operator / (float __a) const
    {
        VertPosNormUvUv b;
        b.position = position / __a;
        b.uv = uv / __a;
        b.uv_glob = uv_glob / __a;
        b.normal = normal / __a;
        return b;
    }
};

/**
 * @brief The VPNTBT struct. Vertex position bytenormal texture
 */
struct VertPosMininormTex{
public:
    glm::vec3 position;
    unsigned char normal;
    glm::vec2 uv;
    VertPosMininormTex(unsigned char __norm, glm::vec3 __pos, glm::vec2 __uv);
    VertPosMininormTex(glm::vec3 __pos, glm::vec2 __uv);
    VertPosMininormTex();
    ~VertPosMininormTex();

    VertPosMininormTex operator + (VertPosMininormTex __a) const
    {
        VertPosMininormTex b;
        b.position = position + __a.position;
        b.uv = __a.uv + uv;
        return b;
    }

    VertPosMininormTex operator / (float __a) const
    {
        VertPosMininormTex b;
       b.position = position / __a;
       b.uv = uv / __a;
       return b;
    }
};

struct VertexPositionColor{
public:
    glm::vec3 pos;
    glm::vec4 col;
};
#endif // VertexPositionTexture_h__
