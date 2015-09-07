/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

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
#include "aabb.h"

template <class _Ty = VertPosNormUvUv>
class UMesh
{
public:
    UMesh() : info(_Ty::info)
    {
    }

    virtual ~UMesh()
    {
        if(vbo)
        {
            glDeleteBuffers(1, &vbo);
        }

        if(ibo)
        {
            glDeleteBuffers(1, &ibo);
        }

        if(vao)
        {
            glBindVertexArray(0);
            glDeleteVertexArrays(1, &vao);
        }

        vao = ibo = vbo = 0;
    }

    template<int aabb_culling = false>
    void Bind()
    {
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

        const GLuint stride = sizeof(_Ty);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_Ty)*vertices.size(), &vertices[0], bindtype);

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

    std::shared_ptr<BasicJargShader> shader;
    std::shared_ptr<Material> material;
    std::vector<_Ty> vertices;
    std::vector<GLuint> indices;
    std::vector<std::string> extensions;
    bool assigned = false;
    bool has_errors = false;
    GLint bindtype = GL_STATIC_DRAW;
    glm::mat4 World = glm::mat4(1);
    VertexInfo info;

    unsigned int loaded_i = 0, loaded_v = 0;
    GLenum primitives = GL_TRIANGLES;

    GLuint vbo = 0, ibo = 0, vao = 0;

    void Assign()
    {
        assert(shader && "need shader to bind");

        for(size_t i = 0; i < info.attrib.size(); ++i)
        {
            const VertexAttribute &a = info.attrib[i];
            a.shader_pos = glGetAttribLocation(shader->program, a.name.c_str());
            if(a.shader_pos == -1)
            {
                LOG(error) << a.name << " in UMesh<" << typeid(_Ty).name() << "> for \"" << shader->shaderfile_name << "\" missed!";
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

    AABB aabb;
    template<typename _Ty, typename T>
    void ComputeAABB(const T _Ty::*field, float scale = 1.0f)
    {
        aabb.builded = false;
        if(!vertices.size())
            return;

        aabb.min = aabb.max = vertices[0].*field;
        for(const auto &a : vertices)
        {
            const auto p = glm::vec3(World * glm::vec4(a.position, 1)) * scale;

            if(aabb.min.x > p.x)
                aabb.min.x = p.x;
            if(aabb.min.y > p.y)
                aabb.min.y = p.y;
            if(aabb.min.z > p.z)
                aabb.min.z = p.z;

            if(aabb.max.x < p.x)
                aabb.max.x = p.x;
            if(aabb.max.y < p.y)
                aabb.max.y = p.y;
            if(aabb.max.z < p.z)
                aabb.max.z = p.z;
        }
        aabb.builded = true;
    }

    template<int aabb_culling = true>
    void Render(const Camera &cam, const glm::mat4 &world = glm::mat4(1))
    {
        if(aabb_culling && aabb.builded)
        {
            if( !cam.BoxWithinFrustum(aabb.min, aabb.max))
                return;
        }

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
        glUniform3fv(shader->lightPosition_location, 1, &glm::vec3(0,1,0)[0]);

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


        if(material)
        {
            if(GLEW_ARB_multi_bind)
            {
                auto texes = material->GetTextureLocations();
                glBindTextures(0, texes.size(), &texes[0]);
            }
            else
            {
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


                if(material && material->detail != nullptr) {
                    glActiveTexture(GL_TEXTURE5);
                    glBindTexture(GL_TEXTURE_2D, material->detail->textureId);
                }
                if(material && material->low != nullptr) {
                    glActiveTexture(GL_TEXTURE6);
                    glBindTexture(GL_TEXTURE_2D, material->low->textureId);
                }
                if(material && material->medium != nullptr) {
                    glActiveTexture(GL_TEXTURE7);
                    glBindTexture(GL_TEXTURE_2D, material->medium->textureId);
                }
                if(material && material->high != nullptr) {
                    glActiveTexture(GL_TEXTURE8);
                    glBindTexture(GL_TEXTURE_2D, material->high->textureId);
                }
                if(material && material->side != nullptr) {
                    glActiveTexture(GL_TEXTURE9);
                    glBindTexture(GL_TEXTURE_2D, material->side->textureId);
                }
            }

            OPENGL_CHECK_ERRORS();
        }

        glBindVertexArray(vao);

        if(primitives == GL_PATCHES) {
            glPatchParameteri(GL_PATCH_VERTICES, 3);
        }

        glDrawElements(primitives, loaded_i, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        OPENGL_CHECK_ERRORS();
    }

    void DrawAABB(const Camera &cam)
    {
        drawWireCube(aabb.min, aabb.max, cam.MVP());
    }
};

#endif // UMESH_H
