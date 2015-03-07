#include "cube.h"
#include <GL/glew.h>
#include "mesh.h"
#include "../colorextender.h"

#define VERTEXCOUNT 24
#define INDEXCOUNT 36

static const glm::vec3 __vertexPositions[VERTEXCOUNT] =
{	
    glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-0.5, 0.5, -0.5),  glm::vec3(0.5, 0.5, -0.5),  glm::vec3(0.5, -0.5, -0.5), // front
    glm::vec3(0.5, -0.5,0.5),    glm::vec3(0.5, 0.5,0.5),     glm::vec3(-0.5, 0.5,0.5),   glm::vec3(-0.5, -0.5,0.5), // back
    glm::vec3(-0.5, 0.5, -0.5),  glm::vec3(-0.5, 0.5,0.5),    glm::vec3(0.5, 0.5,0.5),    glm::vec3(0.5, 0.5, -0.5), // top
    glm::vec3(-0.5, -0.5,0.5),   glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.5, -0.5,0.5), // bottom
    glm::vec3(-0.5, -0.5,0.5),   glm::vec3(-0.5, 0.5,0.5),    glm::vec3(-0.5, 0.5, -0.5), glm::vec3(-0.5, -0.5, -0.5), // left
    glm::vec3(0.5, -0.5, -0.5),  glm::vec3(0.5, 0.5, -0.5),   glm::vec3(0.5, 0.5,0.5),    glm::vec3(0.5, -0.5,0.5) // right
};

static const GLuint __vertexIndex[INDEXCOUNT] =
{
    0, 3, 2, 2, 1, 0, // front
    4, 7, 6, 6, 5, 4, // back
    8,11, 10, 10,9, 8, // top
    12,15,14, 14,13,12, // bottom
    16,19,18, 18,17,16, // left
    20,23,22, 22,21,20 // right
};

std::shared_ptr<Mesh> Cube::getMesh()
{
    if(sm_mesh != nullptr){
        return std::make_shared<Mesh>(*sm_mesh);
    }

    sm_mesh = std::make_shared<Mesh>();

    sm_mesh->Indices.resize(INDEXCOUNT);
    sm_mesh->Vertices.resize(VERTEXCOUNT);
    for(unsigned int i = 0; i < 6; ++i)
    {
        sm_mesh->Vertices[i*4].position = __vertexPositions[i * 4 + 0];
        sm_mesh->Vertices[i*4].uv = glm::vec2(1,1);

        sm_mesh->Vertices[i*4 + 1].position = __vertexPositions[i * 4 + 1];
        sm_mesh->Vertices[i*4 + 1].uv = glm::vec2(1,0);

        sm_mesh->Vertices[i*4 + 2].position = __vertexPositions[i * 4 + 2];
        sm_mesh->Vertices[i*4 + 2].uv = glm::vec2(0,0);

        sm_mesh->Vertices[i*4 + 3].position = __vertexPositions[i * 4 + 3];
        sm_mesh->Vertices[i*4 + 3].uv = glm::vec2(0,1);
    }

    for (int i =0; i< INDEXCOUNT; ++i)
    {
        sm_mesh->Indices.push_back(__vertexIndex[i]);
    }

    return std::make_shared<Mesh>(*sm_mesh);
}

std::shared_ptr<Mesh> Cube::sm_mesh;
