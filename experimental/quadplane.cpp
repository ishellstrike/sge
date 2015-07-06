#include "quadplane.h"
#include <glm/gtx/transform.hpp>

QuadPlane::QuadPlane()
{

}

std::shared_ptr<QuadPlane> &QuadPlane::NW()
{
    return m_parts[NW_PLANE];
}

std::shared_ptr<QuadPlane> &QuadPlane::NE()
{
    return m_parts[NE_PLANE];
}

std::shared_ptr<QuadPlane> &QuadPlane::SW()
{
    return m_parts[SW_PLANE];
}

std::shared_ptr<QuadPlane> &QuadPlane::SE()
{
    return m_parts[SE_PLANE];
}

bool QuadPlane::is_terminal() const
{
    return m_parts[0] == nullptr;
}

void QuadPlane::Render(const glm::mat4 &MVP, std::shared_ptr<Material> &mat, std::shared_ptr<BasicJargShader> &basic)
{
    if(is_terminal())
    {
        if(status == READY)
            terminal_mesh->Render(MVP);
        else
        {
            const int size = 100;
            terminal_mesh->Indices.resize(size*size*6);
            terminal_mesh->Vertices.resize(size*size*4);
            int co = 0;
            for(int j = -size/2; j < size/2; j++)
            {
                for(int i = -size/2; i < size/2; i++)
                {
                    VertPosNormTanBiTex a, b, c, d;

                    a.position = glm::vec3(i    /(float)size, j   /(float)size, 0.5f);
                    b.position = glm::vec3((i+1)/(float)size, j   /(float)size, 0.5f);
                    c.position = glm::vec3((i+1)/(float)size,(j+1)/(float)size, 0.5f);
                    d.position = glm::vec3(i    /(float)size,(j+1)/(float)size, 0.5f);

                    a.position = glm::normalize(a.position);
                    b.position = glm::normalize(b.position);
                    c.position = glm::normalize(c.position);
                    d.position = glm::normalize(d.position);


                    terminal_mesh->Vertices[co*4]   = a;
                    terminal_mesh->Vertices[co*4+1] = b;
                    terminal_mesh->Vertices[co*4+2] = c;
                    terminal_mesh->Vertices[co*4+3] = d;

                    terminal_mesh->Indices[co*6]   = co*4;
                    terminal_mesh->Indices[co*6+1] = co*4+1;
                    terminal_mesh->Indices[co*6+2] = co*4+3;
                    terminal_mesh->Indices[co*6+3] = co*4+1;
                    terminal_mesh->Indices[co*6+4] = co*4+2;
                    terminal_mesh->Indices[co*6+5] = co*4+3;
                    co++;
                }
            }

            terminal_mesh->ForgetBind();
            status = READY;
        }
    }
    else
    {
        for(auto a: m_parts)
        {
            a->Render(MVP, mat, basic);
        }
    }
}

