#include "starfield.h"
#include "random.h"
#include "space/space_solver.h"

Starfield::Starfield()
{
    for(int i = 0; i < 100000; ++i)
    {
        VertPosCol vpc;
        vpc.pos = ssolver::sphere_surface(100.f);
        vpc.col = lerp(Color::White,Color::Clear, random::norm<float>()-0.5f);
        field.vertices.push_back(vpc);
        field.indices.push_back(i);
    }
    field.primitives = GL_POINTS;

    const auto &mat = std::make_shared<Material>();
    std::shared_ptr<BasicJargShader> shader = std::make_shared<BasicJargShader>();
    field.material = mat;
    shader->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/starfield.glsl");
    shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/starfield.glsl");
    shader->Link();
    shader->Afterlink();
    field.shader = shader;

    field.ForgetBind();
}

void Starfield::Render(const Camera &cam)
{
    glPointSize(1);
    field.Render(cam);
}

VertexInfo VertPosCol::info = VertexInfo({VERTEX_ATTRIBUTE("color",    glm::vec4, col, VertPosCol),
                                          VERTEX_ATTRIBUTE("position", glm::vec3, pos, VertPosCol),
                                         });
