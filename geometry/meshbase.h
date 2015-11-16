#ifndef MESHBASE_H
#define MESHBASE_H
#include <glm/glm.hpp>
#include "camera.h"
#include "vertexinfo.h"

class MeshBase
{
public:
    MeshBase(VertexInfo &inf);

    virtual void Bind() = 0;
    virtual void ForgetBind() = 0;
    virtual void __Render(const Camera &cam, const glm::mat4 &mod = glm::mat4(1), bool aabb_culling = false) = 0;
    virtual void Render() = 0;

    VertexInfo info;

    bool assigned = false;
    bool has_errors = false;
    void Assign();

    GLuint vbo = 0, ibo = 0, vao = 0;

    GLint bindtype = GL_STATIC_DRAW;
    glm::mat4 World = glm::mat4(1);
    GLenum primitives = GL_TRIANGLES;


    std::shared_ptr<BasicJargShader> shader;
};

#endif // MESHBASE_H
