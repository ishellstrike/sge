#include "quadplane.h"
#include <glm/gtx/transform.hpp>
#include "ClassicNoise.h"

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

void QuadPlane::Render(const glm::mat4 &MVP, std::shared_ptr<Material> &mat, std::shared_ptr<BasicJargShader> &basic, int side)
{
    if(is_terminal())
    {
        if(status == READY)
            terminal_mesh->Render(MVP);
        else
        {
            terminal_mesh = std::make_shared<Mesh>();
            terminal_mesh->material = mat;
            terminal_mesh->shader = basic;

            const int size = 10;
            terminal_mesh->Indices.resize(size*size*6);
            terminal_mesh->Vertices.resize(size*size*4);
            int co = 0;

            float xs = (-0.5 + offset.x);
            float ys = (-0.5 + offset.y);
            float dd = ((1.0*scale)/(float)size);

            //Generating R=1 sphere
            for(int j = 0; j < size; j++)
            {
                for(int i = 0; i < size; i++)
                {
                    VertPosNormTanBiTex a, b, c, d;

                    a.position = {xs +  i    * dd,
                                  ys +  j    * dd,
                                  0.5f};

                    b.position = {xs + (i+1) * dd,
                                  ys +  j    * dd,
                                  0.5f};

                    c.position = {xs + (i+1) * dd,
                                  ys + (j+1) * dd,
                                  0.5f};

                    d.position = {xs +  i    * dd,
                                  ys + (j+1) * dd,
                                  0.5f};

                    a.position = glm::normalize(a.position)/* + glm::vec3{0, 0, Noise::normalized_simplexnoise(i,  j)  /30.f}*/;
                    b.position = glm::normalize(b.position)/* + glm::vec3{0, 0, Noise::normalized_simplexnoise(i+1,j)  /30.f}*/;
                    c.position = glm::normalize(c.position)/* + glm::vec3{0, 0, Noise::normalized_simplexnoise(i+1,j+1)/30.f}*/;
                    d.position = glm::normalize(d.position)/* + glm::vec3{0, 0, Noise::normalized_simplexnoise(i,  j+1)/30.f}*/;

                    a.normal = a.position;
                    b.normal = b.position;
                    c.normal = c.position;
                    d.normal = d.position;

                    float ux0 = (i/(float)size + 0.5f);
                    float uy0 = (j/(float)size + 0.5f);
                    float du = (1/(float)size);

                    a.uv = {ux0,    uy0}   ;
                    b.uv = {ux0+du, uy0}   ;
                    c.uv = {ux0+du, uy0+du};
                    d.uv = {ux0,    uy0+du};

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

            for(size_t j = 0; j < terminal_mesh->Vertices.size(); j++)
            {
                terminal_mesh->Vertices[j].position =
                        glm::vec3(transformation *
                        glm::vec4(terminal_mesh->Vertices[j].position, 1));

                terminal_mesh->Vertices[j].normal =
                        glm::normalize(
                        glm::vec3(transformation *
                        glm::vec4(terminal_mesh->Vertices[j].normal, 1)));
            }

            subsurface_centers[0] = glm::vec3(transformation *
                                              glm::normalize(glm::vec4(xs + dd * size/4.0,
                                                                       ys + dd * size/4.0,
                                                                       0.5,
                                                                       0)
                                                             )
                                              );
            subsurface_centers[1] = glm::vec3(transformation *
                                              glm::normalize(glm::vec4(xs + dd * size/(3/4.0),
                                                                       ys + dd * size/4.0,
                                                                       0.5,
                                                                       0)
                                                             )
                                              );
            subsurface_centers[2] = glm::vec3(transformation *
                                              glm::normalize(glm::vec4(xs + dd * size/4.0,
                                                                       ys + dd * size/(3/4.0),
                                                                       0.5,
                                                                       0)
                                                             )
                                              );
            subsurface_centers[3] = glm::vec3(transformation *
                                              glm::normalize(glm::vec4(xs + dd * size/(3/4.0),
                                                                       ys + dd * size/(3/4.0),
                                                                       0.5,
                                                                       0)
                                                             )
                                              );

            for(int i = 0; i < 4; ++i)
            {
                subsurface_centers[i] = glm::vec3(transformation * glm::vec4(subsurface_centers[i], 0));
            }

            terminal_mesh->Bind();
            status = READY;
            terminal_mesh->Render(MVP);
        }
    }
    else
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i]->Render(MVP, mat, basic, side);
    }
}

void QuadPlane::Update(Camera &camera, float Rs, float eps)
{
    if(is_terminal() &&
       (glm::distance(subsurface_centers[0] * Rs, camera.getPosition()) < eps * scale ||
        glm::distance(subsurface_centers[1] * Rs, camera.getPosition()) < eps * scale ||
        glm::distance(subsurface_centers[2] * Rs, camera.getPosition()) < eps * scale ||
        glm::distance(subsurface_centers[3] * Rs, camera.getPosition()) < eps * scale)
            && level < 6)
    {
        m_parts[0] = std::make_shared<QuadPlane>();
        m_parts[0]->offset = offset;
        m_parts[0]->scale = scale/2.0f;
        m_parts[0]->level = level + 1;
        m_parts[0]->transformation = transformation;

        m_parts[1] = std::make_shared<QuadPlane>();
        m_parts[1]->offset = offset + glm::vec2(0.5f, 0) * scale;
        m_parts[1]->scale = scale/2.0f;
        m_parts[1]->level = level + 1;
        m_parts[1]->transformation = transformation;

        m_parts[2] = std::make_shared<QuadPlane>();
        m_parts[2]->offset = offset + glm::vec2(0, 0.5f) * scale;
        m_parts[2]->scale = scale/2.0f;
        m_parts[2]->level = level + 1;
        m_parts[2]->transformation = transformation;

        m_parts[3] = std::make_shared<QuadPlane>();
        m_parts[3]->offset = offset + glm::vec2(0.5f, 0.5f) * scale;
        m_parts[3]->scale = scale/2.0f;
        m_parts[3]->level = level + 1;
        m_parts[3]->transformation = transformation;
    }
    else
        if((glm::distance(subsurface_centers[0] * Rs, camera.getPosition()) > eps * 2.0f * scale ||
            glm::distance(subsurface_centers[1] * Rs, camera.getPosition()) > eps * 2.0f * scale ||
            glm::distance(subsurface_centers[2] * Rs, camera.getPosition()) > eps * 2.0f * scale ||
            glm::distance(subsurface_centers[3] * Rs, camera.getPosition()) > eps * 2.0f * scale))
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i] = nullptr;
    }

    if(!is_terminal())
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i]->Update(camera, Rs, eps);
    }
}

