#ifndef VERTEXINFO_H
#define VERTEXINFO_H
#include <vector>
#include <string>
#include <GL/glew.h>

struct VertexAttribute
{
    VertexAttribute(std::string n, GLuint c, GLenum t, GLuint s);
    std::string name = "Vert.position";
    GLuint shader_pos = -1;
    GLuint count = 3;
    GLenum type = GL_FLOAT;
    GLboolean normalized = GL_FALSE;
    GLuint size = sizeof(float) * 3;
};

struct VertexInfo
{
    VertexInfo(std::vector<VertexAttribute> a) : attrib(a){}
    std::vector<VertexAttribute> attrib;
};

#endif // VERTEXINFO_H
