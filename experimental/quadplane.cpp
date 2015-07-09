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

            float xs = (-0.5 + offset.x); /*< x координата начала сектора сферы с отступом*/
            float ys = (-0.5 + offset.y); /*< y координата начала сектора сферы с отступом*/
            float dd = ((1.0*scale)/(float)size); /*< размер сектора сферы*/

            //Генерация R=1 сферы. Нормализуемые плоскости имеют координаты [-0.5, 0.5]. В шейдере свера приводится к радиусу R
            for(int j = 0; j < size; j++)
            {
                for(int i = 0; i < size; i++)
                {
                    VertPosNormTanBiTex a, b, c, d;

                    a.position = {xs +  i    * dd, ys +  j    * dd, 0.5f};
                    b.position = {xs + (i+1) * dd, ys +  j    * dd, 0.5f};
                    c.position = {xs + (i+1) * dd, ys + (j+1) * dd, 0.5f};
                    d.position = {xs +  i    * dd, ys + (j+1) * dd, 0.5f};

                    a.position = glm::normalize(a.position);
                    b.position = glm::normalize(b.position);
                    c.position = glm::normalize(c.position);
                    d.position = glm::normalize(d.position);

                    a.normal = a.position;
                    b.normal = b.position;
                    c.normal = c.position;
                    d.normal = d.position;

                    a.uv = {xs +  i    * dd + 0.5f, ys +  j    * dd + 0.5f};
                    b.uv = {xs + (i+1) * dd + 0.5f, ys +  j    * dd + 0.5f};
                    c.uv = {xs + (i+1) * dd + 0.5f, ys + (j+1) * dd + 0.5f};
                    d.uv = {xs +  i    * dd + 0.5f, ys + (j+1) * dd + 0.5f};

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

            //трансформация вершин сферы в соответствии с матрицей трансформации, заданной при создании объекта сферы
            //матрица трансформации разворачивает части сферы в соответствующие стороны
            for(size_t j = 0; j < terminal_mesh->Vertices.size(); j++)
            {
                terminal_mesh->Vertices[j].normal =
                terminal_mesh->Vertices[j].position =
                        glm::vec3(transformation *
                        glm::vec4(terminal_mesh->Vertices[j].position, 1));
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

            terminal_mesh->ForgetBind();
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
            && level < 8)
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
        if((glm::distance(subsurface_centers[0] * Rs, camera.getPosition()) > eps * 2.0f * scale &&
            glm::distance(subsurface_centers[1] * Rs, camera.getPosition()) > eps * 2.0f * scale &&
            glm::distance(subsurface_centers[2] * Rs, camera.getPosition()) > eps * 2.0f * scale &&
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

