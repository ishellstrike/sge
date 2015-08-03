#ifndef VERTEXINFO_H
#define VERTEXINFO_H
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstddef>

#define FAKE_MEMBER(member_type) ((member_type *)0)
#define VERTEX_ATTRIBUTE(name, member_type, member, type) (VertexAttribute(name, FAKE_MEMBER(member_type), offsetof(type, member)))

struct VertexInfo;

/*!
 * \brief The VertexAttribute struct
 * do not youse direct constructors, use VERTEX_ATTRIBUTE macro, e.g.
 * VertexInfo VertexType::info = VertexInfo({VERTEX_ATTRIBUTE("atr1_name", glm::vec4, atr1, VertexType),
 *                                           VERTEX_ATTRIBUTE("atr2_name", glm::vec3, atr2, VertexType),
 *                                          });
 */
struct VertexAttribute
{
    VertexAttribute(std::string n, float          *, GLuint _offset) : name(n), count(1), type(GL_FLOAT),          offset(_offset){}
    VertexAttribute(std::string n, glm::vec2      *, GLuint _offset) : name(n), count(2), type(GL_FLOAT),          offset(_offset){}
    VertexAttribute(std::string n, glm::vec3      *, GLuint _offset) : name(n), count(3), type(GL_FLOAT),          offset(_offset){}
    VertexAttribute(std::string n, glm::vec4      *, GLuint _offset) : name(n), count(4), type(GL_FLOAT),          offset(_offset){}

    VertexAttribute(std::string n, double         *, GLuint _offset) : name(n), count(1), type(GL_DOUBLE),         offset(_offset){}
    VertexAttribute(std::string n, glm::dvec2     *, GLuint _offset) : name(n), count(2), type(GL_DOUBLE),         offset(_offset){}
    VertexAttribute(std::string n, glm::dvec3     *, GLuint _offset) : name(n), count(3), type(GL_DOUBLE),         offset(_offset){}
    VertexAttribute(std::string n, glm::dvec4     *, GLuint _offset) : name(n), count(4), type(GL_DOUBLE),         offset(_offset){}

    VertexAttribute(std::string n, int            *, GLuint _offset) : name(n), count(1), type(GL_INT),            offset(_offset){}
    VertexAttribute(std::string n, glm::ivec2     *, GLuint _offset) : name(n), count(2), type(GL_INT),            offset(_offset){}
    VertexAttribute(std::string n, glm::ivec3     *, GLuint _offset) : name(n), count(3), type(GL_INT),            offset(_offset){}
    VertexAttribute(std::string n, glm::ivec4     *, GLuint _offset) : name(n), count(4), type(GL_INT),            offset(_offset){}

    VertexAttribute(std::string n, unsigned int   *, GLuint _offset) : name(n), count(1), type(GL_UNSIGNED_INT),   offset(_offset){}
    VertexAttribute(std::string n, glm::uvec2     *, GLuint _offset) : name(n), count(2), type(GL_UNSIGNED_INT),   offset(_offset){}
    VertexAttribute(std::string n, glm::uvec3     *, GLuint _offset) : name(n), count(3), type(GL_UNSIGNED_INT),   offset(_offset){}
    VertexAttribute(std::string n, glm::uvec4     *, GLuint _offset) : name(n), count(4), type(GL_UNSIGNED_INT),   offset(_offset){}

    VertexAttribute(std::string n, bool           *, GLuint _offset) : name(n), count(1), type(GL_BOOL),           offset(_offset){}
    VertexAttribute(std::string n, glm::bvec2     *, GLuint _offset) : name(n), count(2), type(GL_BOOL),           offset(_offset){}
    VertexAttribute(std::string n, glm::bvec3     *, GLuint _offset) : name(n), count(3), type(GL_BOOL),           offset(_offset){}
    VertexAttribute(std::string n, glm::bvec4     *, GLuint _offset) : name(n), count(4), type(GL_BOOL),           offset(_offset){}

    VertexAttribute(std::string n, char           *, GLuint _offset) : name(n), count(1), type(GL_BYTE),           offset(_offset){}

    VertexAttribute(std::string n, unsigned char  *, GLuint _offset) : name(n), count(1), type(GL_UNSIGNED_BYTE),  offset(_offset){}

    VertexAttribute(std::string n, short          *, GLuint _offset) : name(n), count(1), type(GL_SHORT),          offset(_offset){}

    VertexAttribute(std::string n, unsigned short *, GLuint _offset) : name(n), count(1), type(GL_UNSIGNED_SHORT), offset(_offset){}

    std::string name = "error";
    GLuint shader_pos = -1;
    GLuint count = 0;
    GLenum type = 0;
    GLboolean normalized = GL_FALSE;
    GLuint offset = 0;
};

struct VertexInfo
{
    VertexInfo(std::vector<VertexAttribute> a) : attrib(a)
    {

    }
    std::vector<VertexAttribute> attrib;
};

#endif // VERTEXINFO_H
