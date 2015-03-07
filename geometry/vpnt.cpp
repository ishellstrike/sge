#include "vpnt.h"
VertexPositionTexture::VertexPositionTexture(glm::vec3 __v, glm::vec3 __p , glm::vec2 __u) : position(__p), uv(__u)
{
}

VertexPositionTexture::VertexPositionTexture() : position(0), uv(0)
{
}

VertexPositionTexture::~VertexPositionTexture()
{
}

VertPosNormTanBiTex::VertPosNormTanBiTex(glm::vec3 __norm, glm::vec3 __pos , glm::vec2 __uv) : position(__pos), normal(__norm), uv(__uv)
{
}

VertPosNormTanBiTex::VertPosNormTanBiTex(glm::vec3 pos , glm::vec2 __uv) : position(pos), uv(__uv)
{
}

VertPosNormTanBiTex::VertPosNormTanBiTex() : position(), normal(), uv()
{
}

VertPosNormTanBiTex::~VertPosNormTanBiTex()
{
}


VertPosMininormTex::VertPosMininormTex(unsigned char norm, glm::vec3 __pos, glm::vec2 __uv) :
    normal(norm),
    position(__pos),
    uv(__uv)
{

}

VertPosMininormTex::VertPosMininormTex(glm::vec3 __pos, glm::vec2 __uv) :
    position(__pos),
    uv(__uv)
{

}

VertPosMininormTex::VertPosMininormTex()
{

}

VertPosMininormTex::~VertPosMininormTex()
{

}
