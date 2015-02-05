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
struct VertPosNormTanBiTex{
public:
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec3 Binormal;
    glm::vec2 Uv;
    VertPosNormTanBiTex(glm::vec3 norm, glm::vec3 pos, glm::vec2 uv);
    VertPosNormTanBiTex(glm::vec3 pos, glm::vec2 uv);
    VertPosNormTanBiTex();
    ~VertPosNormTanBiTex();

    VertPosNormTanBiTex operator + (VertPosNormTanBiTex a){ VertPosNormTanBiTex b;
                                  b.Position = Position + a.Position;
                                  b.Uv = a.Uv + Uv;
                                  b.Normal = Normal + a.Normal;
                                  b.Tangent = Tangent + a.Tangent;
                                  b.Binormal = Binormal + a.Binormal;
                                  return b; }
    VertPosNormTanBiTex operator / (float a){ VertPosNormTanBiTex b;
                                 b.Position = Position / a;
                                 b.Uv = Uv / a;
                                 b.Normal = Normal / a;
                                 b.Tangent = Tangent / a;
                                 b.Binormal = Binormal / a;
                                 return b; }
};

/**
 * @brief The VPNTBT struct. Vertex position bytenormal texture
 */
struct VertPosMininormTex{
public:
    glm::vec3 Position;
    unsigned char Normal;
    glm::vec2 Uv;
    VertPosMininormTex(unsigned char norm, glm::vec3 pos, glm::vec2 uv);
    VertPosMininormTex(glm::vec3 pos, glm::vec2 uv);
    VertPosMininormTex();
    ~VertPosMininormTex();

    VertPosMininormTex operator + (VertPosMininormTex a){ VertPosMininormTex b;
                                  b.Position = Position + a.Position;
                                  b.Uv = a.Uv + Uv;
                                  return b; }
    VertPosMininormTex operator / (float a){ VertPosMininormTex b;
                                 b.Position = Position / a;
                                 b.Uv = Uv / a;
                                 return b; }
};

struct VertexPositionColor{
public:
    glm::vec3 pos;
    glm::vec4 col;
};
#endif // VertexPositionTexture_h__
