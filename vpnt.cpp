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

VPNT::VPNT(glm::vec3 norm, glm::vec3 pos , glm::vec2 uv) : Position(pos), Normal(norm), Uv(uv)
{
}

VPNT::VPNT() : Position(), Normal(), Uv()
{
}

VPNT::~VPNT()
{
}
