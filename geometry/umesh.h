#ifndef UMESH_H
#define UMESH_H
#include <memory>
#include <vector>
#include "basicjargshader.h"
#include "resources/material.h"
#include "vertexinfo.h"
#include "camera.h"
#include "vpnt.h"
#include "logger.h"

template <class _Ty = VertPosNormUvUv>
class UMesh
{
public:
    UMesh() : info(_Ty::info)
    {
    }

    ~UMesh()
    {
        if(vbo)
        {
            glDeleteBuffers(1, vbo);
            delete[] vbo;
        }

        if(ibo)
        {
            glDeleteBuffers(1, ibo);
            delete[] ibo;
        }

        if(vao)
        {
            glBindVertexArray(0);
            glDeleteVertexArrays(1, vao);

            delete[] vao;
        }

        vao = ibo = vbo = nullptr;
    }
    void Bind()
    {
        assert(shader && "need shader to bind");
        if(!assigned) Assign();

        if(vertices.size() == 0){
            return;
        }

        if(vbo != nullptr) { // rebind
            glBindVertexArray(0);
            for(int i = 0; i < _Ty::info.attrib.size(); ++i)
            {
                const VertexAttribute &a = _Ty::info.attrib[i];
                glDisableVertexAttribArray(a.shader_pos);
            }

            glDeleteBuffers(1, vbo);
            glDeleteBuffers(1, ibo);
            glDeleteVertexArrays(1, vao);
            vao = ibo = vbo = nullptr;
        }

        if(vao == nullptr){
            vao = new GLuint[1];
            glGenVertexArrays(1, vao);
            glBindVertexArray(*vao);
            if(vbo){
                delete[] vbo;
            }
            if(ibo){
                delete[] ibo;
            }
            vbo = new GLuint[1];
            ibo = new GLuint[1];
            glGenBuffers(1, vbo);
            glGenBuffers(1, ibo);
        } else {
            glBindVertexArray(*vao);
        }


        const GLuint stride = sizeof(_Ty);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_Ty)*vertices.size(), &vertices[0], bindtype);

        for(int i = 0; i < info.attrib.size(); ++i)
        {
            const VertexAttribute &a = info.attrib[i];

            glEnableVertexAttribArray(a.shader_pos);
            glVertexAttribPointer(a.shader_pos, a.count, a.type, a.normalized, stride, (void*)(a.offset));
        }


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        if(indices.size())
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices[0]))*indices.size(), &indices[0], bindtype);
        else
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, (void*)0, bindtype);

        loaded_i = indices.size();
        loaded_v = vertices.size();
        glBindVertexArray(0);
    }

    std::shared_ptr<BasicJargShader> shader;
    std::shared_ptr<Material> material;
    std::vector<_Ty> vertices;
    std::vector<GLuint> indices;
    bool assigned = false;
    bool has_errors = false;
    GLint bindtype = GL_STATIC_DRAW;
    glm::mat4 World = glm::mat4(1);
    VertexInfo info;

    unsigned int loaded_i = 0, loaded_v = 0;
    bool patches = false;
    GLenum primitives = GL_TRIANGLES;

    GLuint *vbo = nullptr;
    GLuint *ibo = nullptr;
    GLuint *vao = nullptr;
    void Assign()
    {
        assert(shader && "need shader to bind");

        for(int i = 0; i < info.attrib.size(); ++i)
        {
            const VertexAttribute &a = info.attrib[i];
            a.shader_pos = glGetAttribLocation(shader->program, a.name.c_str());
            if(a.shader_pos == -1)
            {
                LOG(error) << a.name << " missed!";
                has_errors = true;
            }
        }
        assigned = true;
    }
    void ForgetBind()
    {
        Bind();
        indices.clear();
        indices.shrink_to_fit();
        vertices.clear();
        vertices.shrink_to_fit();
    }
    void Render(const Camera &cam, const glm::mat4 &world = glm::mat4(1))
    {
        const glm::mat4 &MVP = cam.MVP();

        if(vertices.size() == 0 && !loaded_v){
            return;
        }
        if(!vao || !vbo)
            return;
        if(!shader)
            throw std::logic_error("null shader");

        shader->Use();
        glUniform3fv(shader->viewPosition_location, 1, &cam.Position()[0]);

        const glm::mat4 Model = world * World;

        glUniformMatrix4fv(shader->mat_model_location, 1, GL_FALSE, &Model[0][0]);
        glUniformMatrix4fv(shader->mat_viewProjection_location, 1, GL_FALSE, &MVP[0][0]);
        const glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(Model)));
        glUniformMatrix3fv(shader->mat_normal_location, 1, GL_FALSE, &normal[0][0]);
        glUniform3fv(shader->lightPosition_location, 1, &glm::vec3(200000,234560,9850000)[0]);

        if(shader->ambient_location != -1)
            glUniform4fv(shader->ambient_location,   1, &material->ambient[0]);
        if(shader->diffuse_location != -1)
            glUniform4fv(shader->diffuse_location,   1, &material->diffuse[0]);
        if(shader->specular_location != -1)
            glUniform4fv(shader->specular_location,  1, &material->specular[0]);
        if(shader->emission_location != -1)
            glUniform4fv(shader->emission_location,  1, &material->emission[0]);
        if(shader->shininess_location != -1)
            glUniform1fv(shader->shininess_location, 1, &material->shininess);

        if(shader->texture_location != -1)
            glUniform1i(shader->texture_location, 0);
        if(shader->normal_location != -1)
            glUniform1i(shader->normal_location, 1);
        if(shader->height_location != -1)
            glUniform1i(shader->height_location, 2);
        if(shader->height_location != -1)
            glUniform1i(shader->grad_location, 3);
        if(shader->global_height_location != -1)
            glUniform1i(shader->global_height_location, 4);

        if(material && material->texture != nullptr) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material->texture->textureId);
        }
        if(material && material->normal != nullptr) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, material->normal->textureId);
        }
        if(material && material->height != nullptr) {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, material->height->textureId);
        }
        if(material && material->grad != nullptr) {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, material->grad->textureId);
        }
        if(material && material->global_height != nullptr) {
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, material->global_height->textureId);
        }

        glBindVertexArray(*vao);

        if(patches) {
            glPatchParameteri(GL_PATCH_VERTICES, 4);
        }

        glDrawElements(primitives, loaded_i, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }
};

#endif // UMESH_H
