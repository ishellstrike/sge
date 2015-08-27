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

VertexInfo VertPosNormUvUv::info = VertexInfo({VERTEX_ATTRIBUTE("position",  glm::vec3, position, VertPosNormUvUv),
                                               VERTEX_ATTRIBUTE("normal",    glm::vec3, normal,   VertPosNormUvUv),
                                               VERTEX_ATTRIBUTE("texcoord",  glm::vec2, uv,       VertPosNormUvUv),
                                               VERTEX_ATTRIBUTE("texcoord2", glm::vec2, uv_glob,  VertPosNormUvUv)
                                              });

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

VertexInfo VertPosUvUv::info = VertexInfo({VERTEX_ATTRIBUTE("position",  glm::vec3, position, VertPosUvUv),
                                           VERTEX_ATTRIBUTE("texcoord",  glm::vec2, uv,       VertPosUvUv),
                                           VERTEX_ATTRIBUTE("texcoord2", glm::vec2, uv_glob,  VertPosUvUv)
                                          });


VertPosUv::VertPosUv(glm::vec3 __pos, glm::vec2 __uv) : position(__pos), uv(__uv)
{

}

VertPosUv::VertPosUv()
{

}

VertPosUv::~VertPosUv()
{

}

VertexInfo VertPosUv::info = VertexInfo({VERTEX_ATTRIBUTE("position",  glm::vec3, position, VertPosUv),
                                         VERTEX_ATTRIBUTE("texcoord",  glm::vec2, uv,       VertPosUv),
                                        });


VertPos::VertPos(glm::vec3 __pos) : position(__pos)
{

}

VertPos::VertPos()
{

}

VertPos::~VertPos()
{

}

VertexInfo VertPos::info = VertexInfo({VERTEX_ATTRIBUTE("position",  glm::vec3, position, VertPosUv)
                                      });

VertPosUvUvBa::VertPosUvUvBa(glm::vec3 __pos, glm::vec2 __uv, glm::vec2 __uv2, glm::vec3 __bary) : position(__pos), uv(__uv), uv_glob(__uv2), bary(__bary)
{

}

VertPosUvUvBa::VertPosUvUvBa()
{

}

VertPosUvUvBa::~VertPosUvUvBa()
{

}

VertexInfo VertPosUvUvBa::info = VertexInfo({VERTEX_ATTRIBUTE("position",    glm::vec3, position, VertPosUvUvBa),
                                             VERTEX_ATTRIBUTE("texcoord",    glm::vec2, uv,       VertPosUvUvBa),
                                             VERTEX_ATTRIBUTE("texcoord2",   glm::vec2, uv_glob,  VertPosUvUvBa),
                                             VERTEX_ATTRIBUTE("barycentric", glm::vec3, bary,     VertPosUvUvBa)
                                            });
