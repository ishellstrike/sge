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

struct VertexAttribute
{
    VertexAttribute(std::string n, glm::vec3 *, GLuint _offset) : name(n), count(3), type(GL_FLOAT), offset(_offset)
    {
    }

    VertexAttribute(std::string n, glm::vec2 *, GLuint _offset) : name(n), count(2), type(GL_FLOAT), offset(_offset)
    {
    }

    VertexAttribute(std::string n, glm::vec4 *, GLuint _offset) : name(n), count(1), type(GL_FLOAT), offset(_offset)
    {
    }

    std::string name = "Vert.position";
    GLuint shader_pos = -1;
    GLuint count = 3;
    GLenum type = GL_FLOAT;
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
