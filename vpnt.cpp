#include "vpnt.h"
VertexPositionTexture::VertexPositionTexture(glm::vec3 v, glm::vec3 p , glm::vec2 u) : Position(p), Uv(u)
{
}

VertexPositionTexture::VertexPositionTexture() : Position(0), Uv(0)
{
}

VertexPositionTexture::~VertexPositionTexture()
{
}

VertPosNormTanBiTex::VertPosNormTanBiTex(glm::vec3 norm, glm::vec3 pos , glm::vec2 uv) : Position(pos), Normal(norm), Uv(uv)
{
}

VertPosNormTanBiTex::VertPosNormTanBiTex(glm::vec3 pos , glm::vec2 uv) : Position(pos), Uv(uv)
{
}

VertPosNormTanBiTex::VertPosNormTanBiTex() : Position(), Normal(), Uv()
{
}

VertPosNormTanBiTex::~VertPosNormTanBiTex()
{
}


VertPosMininormTex::VertPosMininormTex(unsigned char norm, glm::vec3 pos, glm::vec2 uv) :
    Normal(norm),
    Position(pos),
    Uv(uv)
{

}

VertPosMininormTex::VertPosMininormTex(glm::vec3 pos, glm::vec2 uv) :
    Position(pos),
    Uv(uv)
{

}

VertPosMininormTex::VertPosMininormTex()
{

}

VertPosMininormTex::~VertPosMininormTex()
{

}
