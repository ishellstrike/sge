#pragma once
#ifndef Mesh_h__
#define Mesh_h__

#include "vpnt.h"
#include <GL/glew.h>
#include <vector>
#include "basicjargshader.h"
#include "texture.h"
#include <string>
#include "material.h"
#include <memory>
#include "spritebatch.h"

class Mesh
{
public:
    Mesh(void);
    ~Mesh(void);
    void Create(std::vector<VertPosNormTanBiTex> verteces, std::vector<GLuint> indeces);
    void Bind(int type = 0);
    void Render(const glm::mat4 &proj, bool patches = false);
    void Render(const glm::mat4 &Model, const glm::mat4 &proj, bool patches = false);
    void Combine(Mesh* com);
    bool loadOBJ(std::string path);
    void computeNormal();
    void Unindex();
    void MergeVerteces();
    void BuildBounding();

    /** Bind + remove local
     */
    void ForgetBind();
    //void RenderBounding(Batched &sb, mat4 Model);
    glm::vec3 minBound, maxBound;

    std::vector<VertPosNormTanBiTex> Vertices;
    std::vector<GLuint> Indices;
    std::shared_ptr<BasicJargShader> shader;
    std::shared_ptr<Material> material;
    glm::mat4 World;
    std::string id;
    void CalcTB();
    void matf();
    int loaded;
private:
    GLuint *m_vao = nullptr;
    GLuint *m_vbo = nullptr;
};
#endif // Mesh_h__

