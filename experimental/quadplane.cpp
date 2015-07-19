#include "quadplane.h"
#include <glm/gtx/transform.hpp>
#include "ClassicNoise.h"
#include "TextureGenerator.h"
#include "sge_ui/wins.h"

QuadPlane::QuadPlane()
{
}

int GetLevel()
{
    return 1;
}

#define parent_pos \
    PARTS n; \
    if(parent->m_parts[0].get() == this) \
        n = QuadPlane::PARTS::TL; \
    if(parent->m_parts[1].get() == this) \
        n = QuadPlane::PARTS::TR; \
    if(parent->m_parts[2].get() == this) \
        n = QuadPlane::PARTS::DL; \
    if(parent->m_parts[3].get() == this) \
        n = QuadPlane::PARTS::DR;

std::vector<QuadPlane::PARTS> mirrorX(std::vector<QuadPlane::PARTS> &__a)
{
    std::vector<QuadPlane::PARTS> b;
    for(auto a : __a)
    {
        b.push_back(a == QuadPlane::PARTS::TR ?
                         QuadPlane::PARTS::TL :
                    a == QuadPlane::PARTS::TL ?
                         QuadPlane::PARTS::TR :
                    a == QuadPlane::PARTS::DL ?
                         QuadPlane::PARTS::DR :
                         QuadPlane::PARTS::DL);
    }
    return b;
}

std::vector<QuadPlane::PARTS> mirrorY(std::vector<QuadPlane::PARTS> &__a)
{
    std::vector<QuadPlane::PARTS> b;
    for(auto a : __a)
    {
        b.push_back(a == QuadPlane::PARTS::TR ?
                         QuadPlane::PARTS::DR :
                    a == QuadPlane::PARTS::TL ?
                         QuadPlane::PARTS::DL :
                    a == QuadPlane::PARTS::DL ?
                         QuadPlane::PARTS::TL :
                         QuadPlane::PARTS::DR);
    }
    return b;
}

QuadPlane *getRoot(QuadPlane *leaf)
{
    QuadPlane *pre = leaf;
    while(leaf)
    {
        pre = leaf;
        leaf = leaf->parent;
    }
    return pre;
}

QuadPlane *passRoute(QuadPlane *root, std::vector<QuadPlane::PARTS> &route)
{
    std::vector<QuadPlane::PARTS> remain_route = route;
    while(root && remain_route.size())
    {
        QuadPlane::PARTS part = *(--remain_route.end());
        remain_route.pop_back();
        if(root->m_parts[part])
            root = root->m_parts[part].get();
    }
    return root;
}

std::vector<QuadPlane *> QuadPlane::getRoute()
{
    std::vector<QuadPlane *> a;
    if(parent)
    {
        parent_pos

        Neighbours nei_x, nei_y;
        switch(n)
        {
        case TL:
            a.push_back(m_parts[TR].get());a.push_back(m_parts[DL].get());
            nei_x = LEFT_N; nei_y = TOP_N;
            break;
        case TR:
            a.push_back(m_parts[TL].get());a.push_back(m_parts[DR].get());
            nei_x = RIGHT_N; nei_y = TOP_N;
            break;
        case DL:
            a.push_back(m_parts[DR].get());a.push_back(m_parts[TL].get());
            nei_x = LEFT_N; nei_y = BOTTOM_N;
            break;
        case DR:
            a.push_back(m_parts[DL].get());a.push_back(m_parts[TR].get());
            nei_x = RIGHT_N; nei_y = BOTTOM_N;
            break;
        }

        auto root = getRoot(this);

        {
            std::vector<PARTS> pt;
            getRoute(this, pt, nei_x);
            auto xm = mirrorX(pt);
            a.push_back(passRoute(root, xm));
        }
        {
            std::vector<PARTS> pt;
            getRoute(this, pt, nei_y);
            auto ym = mirrorY(pt);
            a.push_back(passRoute(root, ym));
        }
    }
    else
        return {0,0,0,0};
    return a;
}

void QuadPlane::getRoute(QuadPlane *from, std::vector<PARTS> &path, Neighbours that_neib)
{
    if(from->parent)
    {
        parent_pos

        path.push_back(n);
        if((that_neib == TR && n == BOTTOM_N) || (that_neib == TL && n == BOTTOM_N))
            return;
        if((that_neib == TR && n == LEFT_N) || (that_neib == DR && n == LEFT_N))
            return;
        if((that_neib == TL && n == RIGHT_N) || (that_neib == TL && n == RIGHT_N))
            return;
        if((that_neib == DR && n == TOP_N) || (that_neib == DL && n == TOP_N))
            return;
        getRoute(from->parent, path, that_neib);
    }
    return;
}

bool QuadPlane::is_terminal() const
{
    return m_parts[0] == nullptr || m_parts[1] == nullptr || m_parts[2] == nullptr || m_parts[3] == nullptr;
}

//Помещается вершины rldr в меш в прямом порядке
#define PUSH_FORWARD \
    terminal_mesh->Indices.push_back(tl); \
    terminal_mesh->Indices.push_back(tr); \
    terminal_mesh->Indices.push_back(dl); \
    terminal_mesh->Indices.push_back(tr); \
    terminal_mesh->Indices.push_back(dr); \
    terminal_mesh->Indices.push_back(dl);

//Помещается вершины rldr в меш в альтернативном порядке
#define PUSH_BACKWARD \
    terminal_mesh->Indices.push_back(tl); \
    terminal_mesh->Indices.push_back(tr); \
    terminal_mesh->Indices.push_back(dr); \
    terminal_mesh->Indices.push_back(tl); \
    terminal_mesh->Indices.push_back(dr); \
    terminal_mesh->Indices.push_back(dl);

//генерирует индексы ... TODO:описать
#define T_L_D_R_M_STAR \
    int star =        (j)*(size+1) + i; \
    int middle_left = (j)*(size+1) + i - 1; \
    int middle_right = (j)*(size+1) + i + 1; \
    int middle_up =   (j-1)*(size+1) + i; \
    int middle_down =   (j+1)*(size+1) + i; \
\
    int tl =  (j-1)*(size + 1) + i - 1; \
    int tr =  (j-1)*(size + 1) + i + 1; \
    int dl =  (j+1)*(size + 1) + i - 1; \
    int dr =  (j+1)*(size + 1) + i + 1; \


inline void PushTileIndex(Mesh &m, int x, int y, bool top, bool bottom, bool reverse, int sizex)
{
    if(!reverse)
    {
        if(top)
        {
            m.Indices.push_back(y*sizex + x);
            m.Indices.push_back(y*sizex + x+1);
            m.Indices.push_back((y+1)*sizex + x+1);
        }
        if(bottom)
        {
            m.Indices.push_back(y*sizex + x);
            m.Indices.push_back((y+1)*sizex + x+1);
            m.Indices.push_back((y+1)*sizex + x);
        }
    }
    else
    {
        if(top)
        {
            m.Indices.push_back(y*sizex + x);
            m.Indices.push_back(y*sizex + x+1);
            m.Indices.push_back((y+1)*sizex + x);
        }
        if(bottom)
        {
            m.Indices.push_back(y*sizex + x+1);
            m.Indices.push_back((y+1)*sizex + x+1);
            m.Indices.push_back((y+1)*sizex + x);
        }
    }
}

inline void PushTileIndexN(Mesh &m, int x, int y, int part, int sizex, int xoff, int yoff, bool reverse)
{
    if(!reverse)
        switch(part % 4)
        {
        case 0:
            m.Indices.push_back((y+yoff)*sizex + x+xoff);
            m.Indices.push_back((y+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+1+xoff);
            break;
        case 1:
            m.Indices.push_back((y+yoff)*sizex + x+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+xoff);
            break;
        case 2:
            m.Indices.push_back((y+yoff)*sizex + x+xoff);
            m.Indices.push_back((y+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+xoff);
            break;
        case 3:
            m.Indices.push_back((y+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+xoff);
            break;
        }
    else
        switch(part % 4)
        {
        case 0:

            m.Indices.push_back((y+yoff)*sizex + x+xoff);
            m.Indices.push_back((y+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+xoff);
            break;
        case 1:

            m.Indices.push_back((y+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+xoff);
            break;
        case 2:
            m.Indices.push_back((y+yoff)*sizex + x+xoff);
            m.Indices.push_back((y+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+1+xoff);
            break;
        case 3:
            m.Indices.push_back((y+yoff)*sizex + x+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+1+xoff);
            m.Indices.push_back((y+1+yoff)*sizex + x+xoff);
            break;
        }
}

void QuadPlane::GenerateSubTexture(std::shared_ptr<Material> &t, std::shared_ptr<BasicJargShader> &h_shader, std::shared_ptr<BasicJargShader> &g_shader)
{
    const float res = 256.0f;
    TextureGenerator tg;
    std::shared_ptr<Texture> tnoise = std::make_shared<Texture>();
    std::shared_ptr<Texture> dnoise = std::make_shared<Texture>();
    std::shared_ptr<Texture> height_map = std::make_shared<Texture>(glm::vec2{res,res});
    std::shared_ptr<Texture> grad_map = std::make_shared<Texture>(glm::vec2{res,res});
    tnoise->Load("data/PerlinPerm2D.png");
    dnoise->Load("data/PerlinGrad2D.png");
    tg.SetShader(h_shader);
    tg.AddTexture("samplerPerlinPerm2D", tnoise);
    tg.AddTexture("samplerPerlinGrad2D", dnoise);
    tg.SetParams(offset.x);
    tg.SetParams(offset.y);
    tg.SetParams(scale);
    tg.SetResultTexture(height_map);
    tg.RenderOnTempFbo();

    t->height = height_map;

    TextureGenerator grad_gen;
    grad_gen.SetShader(g_shader);
    grad_gen.AddTexture("height_map", height_map);
    grad_gen.SetResultTexture(grad_map);
    grad_gen.SetParams(height_map->width);
    grad_gen.SetParams(height_map->height);
    grad_gen.RenderOnTempFbo();

    t->grad = grad_map;
}

void QuadPlane::Render(const Camera &cam,
                       std::shared_ptr<Material> &mat,
                       std::shared_ptr<BasicJargShader> &basic,
                       int side,
                       std::shared_ptr<BasicJargShader> &h_shader,
                       std::shared_ptr<BasicJargShader> &g_shader)
{
    if(is_terminal())
    {
        if(status == READY)
        {
            terminal_mesh->Render(cam);
            //WinS::ws->sb->drawQuad(glm::vec2(10,10) + 500.f*offset, glm::vec2(500,500)*scale, *terminal_mesh->material->height, Color::White);
        }
        else
        {
            std::shared_ptr<Material> sub_texture = std::make_shared<Material>();
            GenerateSubTexture(sub_texture, h_shader, g_shader);
            sub_texture->texture = sub_texture->grad;

            terminal_mesh->material = sub_texture;
            terminal_mesh->shader = basic;

            const int size = 32;
            terminal_mesh->Indices.reserve(size * size * 6);
            terminal_mesh->Vertices.reserve((size + 1) * (size + 1));
            int xcount = size + 1;
            int co = 0;

            float xs = (-0.5 + offset.x); /*< x координата начала сектора сферы с отступом*/
            float ys = (-0.5 + offset.y); /*< y координата начала сектора сферы с отступом*/
            float dd = ((1.0*scale)/((float)size)); /*< размер 1 квада сектора сферы*/

            //Генерация R=1 сферы. Нормализуемые плоскости имеют координаты [-0.5, 0.5]. В шейдере сфера приводится к радиусу R

            for(int j = 0; j < size + 1; j++)
            {
                for(int i = 0; i < size + 1; i++)
                {
                    VertPosNormTanBiTex a;
                    a.position = {xs + i * dd, ys + j * dd, 0.5f};

                    a.position = glm::normalize(a.position);

                    a.normal = a.position;

                    a.uv = {i * (1.0/(float)size), j * (1.0/(float)size) };

                    terminal_mesh->Vertices.push_back(a);
                }
            }

            std::vector<QuadPlane *> nighbours = getRoute();

            //top
            int p = 0;
            for(int i=0;i<size;i+=2)
            {
                int j = 0;

                bool no_nei = !nighbours[TOP_N] || nighbours[TOP_N]->level >= level;

                if(i!=0)
                    PushTileIndexN(*terminal_mesh, i, j, 1, xcount, 0, 0, false);
                if(no_nei)
                {
                    PushTileIndexN(*terminal_mesh, i, j, 0, xcount, 0, 0, false);
                    PushTileIndexN(*terminal_mesh, i, j, 2, xcount, 1, 0, false);
                }
                else if (i % 2 == 0)
                {
                    terminal_mesh->Indices.push_back((j)*xcount + i);
                    terminal_mesh->Indices.push_back((j)*xcount + i + 2);
                    terminal_mesh->Indices.push_back((j + 1)*xcount + i + 1);
                }
                if(i!=size-2)
                    PushTileIndexN(*terminal_mesh, i, j, 3, xcount, 1, 0, false);
            }

            //bot
            p = 0;
            for(int i=0;i<size;i+=2)
            {
                int j = size-1;

                bool no_nei = !nighbours[BOTTOM_N] || nighbours[BOTTOM_N]->level >= level;

                if(i!=0)
                    PushTileIndexN(*terminal_mesh, i, j, 0, xcount, 0, 0, true);
                if(no_nei)
                {
                    PushTileIndexN(*terminal_mesh, i, j, 1, xcount, 0, 0, true);
                    PushTileIndexN(*terminal_mesh, i, j, 3, xcount, 1, 0, true);
                }
                else if (i % 2 == 0)
                {
                    terminal_mesh->Indices.push_back((j)*xcount + i + 1);
                    terminal_mesh->Indices.push_back((j + 1)*xcount + i + 2);
                    terminal_mesh->Indices.push_back((j + 1)*xcount + i);
                }
                if(i!=size-2)
                    PushTileIndexN(*terminal_mesh, i, j, 2, xcount, 1, 0, true);
            }

            //left
            p = 0;
            for(int j=0;j<size;j+=2)
            {
                int i = 0;

                bool no_nei = !nighbours[LEFT_N] || nighbours[LEFT_N]->level >= level;

                if(j!=0)
                    PushTileIndexN(*terminal_mesh, i, j, 0, xcount, 0, 0, false);
                if(no_nei)
                {
                    PushTileIndexN(*terminal_mesh, i, j, 1, xcount, 0, 0, false);
                    PushTileIndexN(*terminal_mesh, i, j, 2, xcount, 0, 1, false);
                }
                else if (j % 2 == 0)
                {
                    terminal_mesh->Indices.push_back((j)*xcount + i);
                    terminal_mesh->Indices.push_back((j + 1)*xcount + i + 1);
                    terminal_mesh->Indices.push_back((j + 2)*xcount + i);
                }
                if(j!=size-2)
                    PushTileIndexN(*terminal_mesh, i, j, 3, xcount, 0, 1, false);
            }

            //right
            p = 0;
            for(int j=0;j<size;j+=2)
            {
                int i = size - 1;

                bool no_nei = !nighbours[RIGHT_N] || nighbours[RIGHT_N]->level >= level;

                if(j!=0)
                    PushTileIndexN(*terminal_mesh, i, j, 0, xcount, 0, 0, true);
                if(no_nei)
                {
                    PushTileIndexN(*terminal_mesh, i, j, 1, xcount, 0, 0, true);
                    PushTileIndexN(*terminal_mesh, i, j, 2, xcount, 0, 1, true);
                }
                else if (j % 2 == 0)
                {
                    terminal_mesh->Indices.push_back((j)*xcount + i + 1);
                    terminal_mesh->Indices.push_back((j + 2)*xcount + i + 1);
                    terminal_mesh->Indices.push_back((j + 1)*xcount + i);
                }
                if(j!=size-2)
                    PushTileIndexN(*terminal_mesh, i, j, 3, xcount, 0, 1, true);
            }

            for(int j = 1; j < size - 1; j+=2)
            {
                for(int i = 1; i < size - 1; ++i)
                {
                    int tl = j*(size + 1) + i;
                    int tr = j*(size + 1) + i + 1;
                    int dl = (j+1)*(size + 1) + i;
                    int dr = (j+1)*(size + 1) + i + 1;

                    PUSH_FORWARD
                }

                for(int i = size - 2; i >= 1; --i)
                {
                    int tl = (j+1)*(size + 1) + i;
                    int tr = (j+1)*(size + 1) + i + 1;
                    int dl = (j+2)*(size + 1) + i;
                    int dr = (j+2)*(size + 1) + i + 1;

                    PUSH_BACKWARD
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

            terminal_mesh->ForgetBind();
            status = READY;
            terminal_mesh->Render(cam);
        }
    }
    else
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i]->Render(cam, mat, basic, side, h_shader, g_shader);
    }

}

#undef T_L_D_R_M_STAR
#undef PUSH_BACKWARD
#undef PUSH_FORWARD

void QuadPlane::Update(Camera &camera, float Rs, float eps, int max_divide)
{
    if(is_terminal() &&
       (glm::distance(subsurface_centers[0] * Rs, camera.Position()) < eps * scale ||
        glm::distance(subsurface_centers[1] * Rs, camera.Position()) < eps * scale ||
        glm::distance(subsurface_centers[2] * Rs, camera.Position()) < eps * scale ||
        glm::distance(subsurface_centers[3] * Rs, camera.Position()) < eps * scale)
            && level < max_divide)
    {
        m_parts[0] = std::make_shared<QuadPlane>();
        m_parts[0]->terminal_mesh = std::make_shared<Mesh>();
        m_parts[0]->offset = offset;
        m_parts[0]->scale = scale/2.0f;
        m_parts[0]->level = level + 1;
        m_parts[0]->transformation = transformation;
        m_parts[0]->parent = this;

        m_parts[1] = std::make_shared<QuadPlane>();
        m_parts[1]->terminal_mesh = std::make_shared<Mesh>();
        m_parts[1]->offset = offset + glm::vec2(0.5f, 0) * scale;
        m_parts[1]->scale = scale/2.0f;
        m_parts[1]->level = level + 1;
        m_parts[1]->transformation = transformation;
        m_parts[1]->parent = this;

        m_parts[2] = std::make_shared<QuadPlane>();
        m_parts[2]->terminal_mesh = std::make_shared<Mesh>();
        m_parts[2]->offset = offset + glm::vec2(0, 0.5f) * scale;
        m_parts[2]->scale = scale/2.0f;
        m_parts[2]->level = level + 1;
        m_parts[2]->transformation = transformation;
        m_parts[2]->parent = this;

        m_parts[3] = std::make_shared<QuadPlane>();
        m_parts[3]->terminal_mesh = std::make_shared<Mesh>();
        m_parts[3]->offset = offset + glm::vec2(0.5f, 0.5f) * scale;
        m_parts[3]->scale = scale/2.0f;
        m_parts[3]->level = level + 1;
        m_parts[3]->transformation = transformation;
        m_parts[3]->parent = this;

        //status = ERROR;
    }
    else
        if((glm::distance(subsurface_centers[0] * Rs, camera.Position()) > eps * 1.1f * scale &&
            glm::distance(subsurface_centers[1] * Rs, camera.Position()) > eps * 1.1f * scale &&
            glm::distance(subsurface_centers[2] * Rs, camera.Position()) > eps * 1.1f * scale &&
            glm::distance(subsurface_centers[3] * Rs, camera.Position()) > eps * 1.1f * scale))
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i] = nullptr;
    }

    if(!is_terminal())
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i]->Update(camera, Rs, eps, max_divide);
    }
}

