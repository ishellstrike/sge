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
#include "meshbase.h"

struct UMeshDc
{
    static int dc, last_dc;
    static void resetDc();
    static int getDc();
};

template <class _Vert = VertPosNormUvUv>
class UMesh : public MeshBase
{
public:
    UMesh() : MeshBase(_Vert::info)
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

    std::shared_ptr<Material> material;
    std::vector<_Vert> vertices;
    std::vector<GLuint> indices;

    GLint bindtype = GL_STATIC_DRAW;
    glm::mat4 World = glm::mat4(1);
    GLenum primitives = GL_TRIANGLES;

    unsigned int loaded_i = 0, loaded_v = 0;

    void Bind() override
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

        const GLuint stride = sizeof(_Vert);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_Vert)*vertices.size(), &vertices[0], bindtype);

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

    void ForgetBind() override
    {
        Bind();
        indices.clear();
        indices.shrink_to_fit();
        vertices.clear();
        vertices.shrink_to_fit();
    }

    AABB aabb;
    template<typename _Ty, typename T>
    void ComputeAABB(const T _Ty::*field)
    {
        aabb.builded = false;
        if(!vertices.size())
            return;

        aabb.min = aabb.max = vertices[0].*field;
        for(const auto &a : vertices)
        {
            const auto p = glm::vec3(World * glm::vec4(a.position, 1));

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

    void Render(const Camera &cam, const glm::mat4 &mod = glm::mat4(1), bool aabb_culling = false) override
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

        glm::mat4 model = World * mod;
        glUniformMatrix4fv(shader->mat_model_location, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(shader->mat_viewProjection_location, 1, GL_FALSE, &MVP[0][0]);

        glm::mat3 normal = glm::transpose(glm::mat3(glm::inverse(model)));
        glUniformMatrix3fv(shader->mat_normal_location, 1, GL_FALSE, &normal[0][0]);
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
        UMeshDc::dc++;
    }

    void DrawAABB(const Camera &cam)
    {
        drawWireCube(aabb.min, aabb.max, cam.MVP());
    }

    void clean()
    {
        std::vector<_Vert> v = std::vector<_Vert>();
        v.reserve(vertices.size());
        std::vector<GLuint> ii = std::vector<GLuint>();
        ii.reserve(indices.size());

        for(size_t i=0; i < vertices.size();i+=3){
            if(vertices[i].position != vertices[i+1].position && vertices[i+1].position != vertices[i+2].position && vertices[i].position != vertices[i+2].position)
            {
                v.push_back(vertices[i]);
                v.push_back(vertices[i+1]);
                v.push_back(vertices[i+2]);

                ii.push_back(v.size()-3);
                ii.push_back(v.size()-2);
                ii.push_back(v.size()-1);
            }
        }
        vertices = v;
        indices = ii;
    }

    void computeNormal()
    {
        for(size_t i=0; i < vertices.size();i+=3){
            glm::vec3 const & a = vertices[i].position;
            glm::vec3 const & b = vertices[i+1].position;
            glm::vec3 const & c = vertices[i+2].position;
            auto t = glm::normalize(glm::cross(c - a, b - a));
            vertices[i].normal = vertices[i+1].normal = vertices[i+2].normal = t;
        }
    }

    void MergeVerteces()
    {
        for (size_t i=0;i<vertices.size();i++)
        {
            auto n = vertices[i].normal;
            float nn = 1.0f;
            std::vector<int> same;
            same.reserve(10);
            for (size_t j=i+1;j<vertices.size();j++)
            {
                if(vertices[i].position == vertices[j].position){
                    n += vertices[j].normal;
                    nn++;
                    same.push_back(j);
                }
            }
            n /= nn;
            vertices[i].normal = n;
            if(same.size() > 0)
                for(size_t j=0; j<same.size(); j++){
                    vertices[same[j]].normal = n;
                }
        }
    }

    void Unindex()
    {
        auto temp = std::move(vertices);
        vertices.resize(indices.size());
        for(int i=0; i<indices.size();i++){
            vertices[i] = temp[indices[i]];
            indices[i] = i;
        }
    }
};

#endif // UMESH_H
