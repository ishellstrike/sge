#include "quadsphere.h"
#include "geometry/icosahedron.h"
#include "glm/gtx/transform.hpp"
#include "TextureGenerator.h"

//Помещается вершины rldr в меш в прямом порядке
#define PUSH_FORWARD \
    Indeces.push_back(tl); \
    Indeces.push_back(tr); \
    Indeces.push_back(dl); \
    Indeces.push_back(tr); \
    Indeces.push_back(dr); \
    Indeces.push_back(dl);

//Помещается вершины rldr в меш в альтернативном порядке
#define PUSH_BACKWARD \
    Indeces.push_back(tl); \
    Indeces.push_back(tr); \
    Indeces.push_back(dr); \
    Indeces.push_back(tl); \
    Indeces.push_back(dr); \
    Indeces.push_back(dl);

QuadSphere::QuadSphere(std::shared_ptr<BasicJargShader> &shader, std::shared_ptr<Material> &__mat)
{
    basic = shader;
    mat = __mat;

    for(int i = 0; i < 6; i++)
    {
        plane[i] = std::make_shared<QuadPlane>();
        plane[i]->terminal_mesh = std::make_shared< UMesh<VertPosUvUv> >();
    }


    plane[1]->transformation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0,1,0));
    plane[2]->transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(0,1,0));
    plane[3]->transformation = glm::rotate(glm::mat4(1), -glm::half_pi<float>(), glm::vec3(0,1,0));
    plane[4]->transformation = glm::rotate(glm::mat4(1), -glm::half_pi<float>(), glm::vec3(1,0,0));
    plane[5]->transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1,0,0));

    height_shader = std::make_shared<BasicJargShader>();
    height_shader->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/testgen1.glsl");
    height_shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/testgen1.glsl");
    height_shader->Link();
    height_shader->Afterlink();

    grad_shader = std::make_shared<BasicJargShader>();
    grad_shader->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/gradient_builder.glsl");
    grad_shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/gradient_builder.glsl");
    grad_shader->Link();
    grad_shader->Afterlink();

    shader_r = glGetUniformLocation(basic->program, "R");
    shader_s = glGetUniformLocation(basic->program, "s");

    const float res = 2048.0f;
    TextureGenerator tg;
    std::shared_ptr<Texture> height_map = std::make_shared<Texture>(glm::vec2{res,res}, false);

    noise_map = std::make_shared<Texture>();
    grad_map = std::make_shared<Texture>();
    noise_map->Load("data/textures/PerlinPerm2D.png");
    grad_map->Load("data/textures/PerlinGrad2D.png");


    tg.SetShader(height_shader);
    tg.AddTexture("samplerPerlinPerm2D", noise_map);
    tg.AddTexture("samplerPerlinGrad2D", grad_map);
    tg.SetParams(0);
    tg.SetParams(0);
    tg.SetParams(1);
    tg.SetResultTexture(height_map);
    tg.RenderOnTempFbo();

    mat->global_height = height_map;


    int xcount = tess_size + 3;
    for(int j = 0; j < tess_size + 2; j+=2)
    {
        for(int i = 0; i < tess_size + 2; ++i)
        {
            int tl = j*xcount + i;
            int tr = j*xcount + i + 1;
            int dl = (j+1)*xcount + i;
            int dr = (j+1)*xcount + i + 1;

            if(i%2==0)
            {
                PUSH_FORWARD
            }
            else
            {
                PUSH_BACKWARD
            }
        }

        for(int i = 0; i < tess_size + 2; ++i)
        {
            int tl = (j+1)*xcount + i;
            int tr = (j+1)*xcount + i + 1;
            int dl = (j+2)*xcount + i;
            int dr = (j+2)*xcount + i + 1;

            if(i%2==0)
            {
                PUSH_BACKWARD
            }
            else
            {
                PUSH_FORWARD
            }
        }
    }

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*Indeces.size(), &Indeces[0], GL_STATIC_DRAW);
}

QuadSphere::~QuadSphere()
{
    glDeleteBuffers(1, &ibo);
}

#undef PUSH_BACKWARD
#undef PUSH_FORWARD

void QuadSphere::Render(const Camera &cam)
{
    for(int i = 0; i < 6; i++)
    {
        plane[i]->Render(cam, i, this);
    }
}

void QuadSphere::SetWorld(const glm::mat4 &world)
{

}

void QuadSphere::Update(Camera &camera)
{
    if(pow(center.x - camera.Position().x, 2) + pow(center.y - camera.Position().y, 2) + pow(center.z - camera.Position().z, 2) < R*R)
        return;
    for(int i = 0; i < 6; i++)
    {
        plane[i]->Update(camera, R + s, R * 2, max_divide, this);
    }
}

