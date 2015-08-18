#ifndef MODEL_H
#define MODEL_H
#include "umesh.h"
#include "../resources/material.h"
#include "../basicjargshader.h"

class Model
{
public:
    Model();
    std::vector<UMesh<int>> meshes;
    std::vector<Material> materials;
    std::vector<BasicJargShader> shaders;

    std::vector<std::string> names;
    std::unordered_map<std::string, size_t> dictionary;

    void Bind(int type = 0);
    void Render(const glm::mat4 &proj, bool patches = false);
    void Render(const glm::mat4 &Model, const glm::mat4 &proj, bool patches = false);
    bool Load(const std::string &path);
    void Clear();
};

#endif // MODEL_H
