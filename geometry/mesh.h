#pragma once
#ifndef Mesh_h__
#define Mesh_h__

#include "vpnt.h"
#include <GL/glew.h>
#include <vector>
#include "../basicjargshader.h"
#include "../resources/texture.h"
#include <string>
#include "../resources/material.h"
#include <memory>
#include "../spritebatch.h"


/*!
 * \brief The Mesh class
 *
 * Represent simplest renderable object
 * contains personal #Material and #BasicJargShader
 */
class Mesh
{
public:
    Mesh(void);
    Mesh(const Mesh &m);
    ~Mesh(void);
    void Create(std::vector<VertPosNormTanBiTex> verteces, std::vector<GLuint> indeces);
    void Bind(int type = 0);
    void Render(const glm::mat4 &MVP, bool patches = false);
    void Render(const glm::mat4 &Model, const glm::mat4 &VP, bool patches = false);
    void Combine(Mesh* com);
    bool loadOBJ(const std::string &path);
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
    glm::mat4 World = glm::mat4(1);
    std::string id = "";
    void CalcTB();
    void matf();
    int loaded = false;
    void loadSTL(const std::string &path);
    float FarestPoint();
    bool loadMTL(const std::string &path);
private:
    GLuint *m_vao = nullptr;
    GLuint *m_vbo = nullptr;
};
#endif // Mesh_h__

