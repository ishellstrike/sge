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

VertPosNormUvUv::VertPosNormUvUv(glm::vec3 __norm, glm::vec3 __pos, glm::vec2 __uv) : position(__pos), normal(__norm), uv(__uv)
{

}

VertPosNormUvUv::VertPosNormUvUv(glm::vec3 __norm, glm::vec3 __pos , glm::vec2 __uv, glm::vec2 __uv2) : position(__pos), normal(__norm), uv(__uv), uv_glob(__uv2)
{
}

VertPosNormUvUv::VertPosNormUvUv(glm::vec3 pos , glm::vec2 __uv) : position(pos), uv(__uv)
{
}

VertPosNormUvUv::VertPosNormUvUv() : position(), normal(), uv()
{
}

VertPosNormUvUv::~VertPosNormUvUv()
{
}

VertexInfo VertPosNormUvUv::info = VertexInfo({VertexAttribute("position", 3, GL_FLOAT, sizeof(glm::vec3)),
                                               VertexAttribute("normal", 3, GL_FLOAT, sizeof(glm::vec3)),
                                               VertexAttribute("texcoord", 2, GL_FLOAT, sizeof(glm::vec2)),
                                               VertexAttribute("texcoord2", 2, GL_FLOAT, sizeof(glm::vec2))});

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


VertPosUvUv::VertPosUvUv(glm::vec3 __pos, glm::vec2 __uv) : uv(__uv), position(__pos)
{

}

VertPosUvUv::VertPosUvUv(glm::vec3 __pos, glm::vec2 __uv, glm::vec2 __uv2) : uv(__uv), position(__pos), uv_glob(__uv2)
{

}

VertPosUvUv::VertPosUvUv()
{

}

VertPosUvUv::~VertPosUvUv()
{

}

VertexInfo VertPosUvUv::info = VertexInfo({VertexAttribute("position", 3, GL_FLOAT, sizeof(float)*3),
                                           VertexAttribute("texcoord", 2, GL_FLOAT, sizeof(float)*2),
                                           VertexAttribute("texcoord2", 2, GL_FLOAT, sizeof(float)*2)});
