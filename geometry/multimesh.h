#ifndef MULTIMESH_H
#define MULTIMESH_H
#include "meshbase.h"
#include "vpnt.h"
#include "memorychunkcontroller.h"
#include <list>
#include "umesh.h"

template <typename _Vert = VertPosNormUvUv>
class MultiMesh : public MeshBase
{
public:
    const size_t multisize = 100 * (2<<30);
    const size_t vertexcount = multisize / sizeof(_Vert);

    MultiMesh()
    {
        mem(multisize);

        assert(shader && "need shader to bind");
        if(!assigned) Assign();

        if(vertices.size() == 0){
            return;
        }

        if(vao) { // rebind
            glBindVertexArray(0);

            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ibo);
            glDeleteVertexArrays(1, &vao);
            vao = ibo = vbo = 0;
        }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        const GLuint stride = sizeof(_Vert);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, multisize, (void*)0, bindtype);

        for(size_t i = 0; i < info.attrib.size(); ++i)
        {
            const VertexAttribute &a = info.attrib[i];

            glEnableVertexAttribArray(a.shader_pos);
            glVertexAttribPointer(a.shader_pos, a.count, a.type, a.normalized, stride, (void*)(a.offset));
        }


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        if(indices.size())
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices[0]))*indices.size(), &indices[0], bindtype);
        else
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, (void*)0, bindtype);

        loaded_i = indices.size();
        loaded_v = vertices.size();
        glBindVertexArray(0);

        OPENGL_CHECK_ERRORS();
    }

    MemoryChunkController mem;
    std::list<std::shared_ptr<UMesh<_Vert>>> meshes;
    std::vector<_Vert> vertices; //fake
    std::vector<GLuint> indices; //fake

    // MeshBase interface
public:
    void Bind() override
    {
        static_assert(false, "Multimesh can't be binded directly. Use Add()");
    }

    void ForgetBind() override
    {
        static_assert(false, "Multimesh can't be binded directly. Use Add()");
    }

    void Render(const Camera &cam, const glm::mat4 &mod = glm::mat4(1), bool aabb_culling = false) override
    {

    }

    bool Add(std::shared_ptr<UMesh<_Vert>> &val)
    {
        if(!mem.Store(&val->vertices[0], sizeof(decltype(val->vertices[0]))*val->vertices.size()))
            return false;

        meshes.push_back(val);
    }
};

#endif // MULTIMESH_H
