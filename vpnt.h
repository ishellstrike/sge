#pragma once
#ifndef VertexPositionTexture_h__
#define VertexPositionTexture_h__

#include "colorextender.h"
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
    glm::vec3 Position;
    glm::vec2 Uv;
    VertexPositionTexture(glm::vec3 v, glm::vec3 p , glm::vec2 u);
    VertexPositionTexture();
    ~VertexPositionTexture();

    VertexPositionTexture operator + (VertexPositionTexture a){ VertexPositionTexture b; b.Position = Position + a.Position; b.Uv = a.Uv + Uv; return b; }
    VertexPositionTexture operator / (float a){ VertexPositionTexture b; b.Position = Position / a; b.Uv = Uv / a; return b; }
};

/**
 * @brief The VPNTBT struct. Vertex position normal tangent binormal texture
 */
struct VPNTBT{
public:
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec3 Binormal;
    glm::vec2 Uv;
    VPNTBT(glm::vec3 norm, glm::vec3 pos, glm::vec2 uv);
    VPNTBT(glm::vec3 pos, glm::vec2 uv);
    VPNTBT();
    ~VPNTBT();

    VPNTBT operator + (VPNTBT a){ VPNTBT b;
                                  b.Position = Position + a.Position;
                                  b.Uv = a.Uv + Uv;
                                  b.Normal = Normal + a.Normal;
                                  b.Tangent = Tangent + a.Tangent;
                                  b.Binormal = Binormal + a.Binormal;
                                  return b; }
    VPNTBT operator / (float a){ VPNTBT b;
                                 b.Position = Position / a;
                                 b.Uv = Uv / a;
                                 b.Normal = Normal / a;
                                 b.Tangent = Tangent / a;
                                 b.Binormal = Binormal / a;
                                 return b; }
};

struct VertexPositionColor{
public:
    glm::vec3 pos;
    glm::vec4 col;
};
#endif // VertexPositionTexture_h__
