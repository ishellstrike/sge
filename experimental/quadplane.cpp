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

void QuadPlane::GenerateSubTexture(std::shared_ptr<Material> &t, SphereParamsStorage *parent)
{
    const float res = 512.0f;
    TextureGenerator tg;
    std::shared_ptr<Texture> height_map = std::make_shared<Texture>(glm::vec2{res,res});
    std::shared_ptr<Texture> grad_map = std::make_shared<Texture>(glm::vec2{res,res});


    tg.SetShader(parent->height_shader);
    tg.AddTexture("samplerPerlinPerm2D", parent->noise_map);
    tg.AddTexture("samplerPerlinGrad2D", parent->grad_map);
    tg.SetParams(offset.x);
    tg.SetParams(offset.y);
    tg.SetParams(scale);
    tg.SetResultTexture(height_map);
    tg.RenderOnTempFbo();

    t->height = height_map;

    TextureGenerator grad_gen;
    grad_gen.SetShader(parent->grad_shader);
    grad_gen.AddTexture("height_map", height_map);
    grad_gen.SetResultTexture(grad_map);
    grad_gen.SetParams(height_map->width);
    grad_gen.SetParams(height_map->height);
    grad_gen.SetParams(scale);
    grad_gen.RenderOnTempFbo();

    t->grad = grad_map;
}

void QuadPlane::Render(const Camera &cam,
                       int side,
                       SphereParamsStorage *parent)
{
    if(is_terminal())
    {
        if(status == READY)
        {
            terminal_mesh->Render(cam, parent->world);
            float pre_size = 222.f;
            //texture vis
            //WinS::ws->sb->drawQuad(glm::vec2(10,10) + pre_size*offset + glm::vec2(pre_size,0)*(float)(side/2) + glm::vec2(0,pre_size)*(float)(side%2), glm::vec2(pre_size)*scale, *terminal_mesh->material->grad, Color::White);
            //quadtree vis
            //WinS::ws->sb->drawRect(glm::vec2(10,10) + pre_size*offset + glm::vec2(pre_size,0)*(float)(side/2) + glm::vec2(0,pre_size)*(float)(side%2), glm::vec2(pre_size)*scale, glm::vec4(rand()%255/255.0,rand()%255/255.0,rand()%255/255.0,1 ));
        }
        else
        {
            std::shared_ptr<Material> sub_texture = std::make_shared<Material>();
            GenerateSubTexture(sub_texture, parent);
            sub_texture->texture = parent->mat->texture;
            sub_texture->global_height = parent->mat->global_height;

            terminal_mesh->material = sub_texture;
            terminal_mesh->shader = parent->basic;

            int size = parent->tess_size;
            terminal_mesh->Indices.reserve(size * size * 6);
            terminal_mesh->Vertices.reserve((size + 1) * (size + 1));
            int co = 0;

            float xs = (-0.5 + offset.x); /*< x координата начала сектора сферы с отступом*/
            float ys = (-0.5 + offset.y); /*< y координата начала сектора сферы с отступом*/
            float dd = ((1.0*scale)/((float)size)); /*< размер 1 квада сектора сферы*/

            //Генерация R=1 сферы. Нормализуемые плоскости имеют координаты [-0.5, 0.5]. В шейдере сфера приводится к радиусу R

            for(int j = -1; j < size + 2; j++)
            {
                for(int i = -1; i < size + 2; i++)
                {
                    VertPosNormUvUv a;
                    a.position = {xs + i * dd, ys + j * dd, 0.5f};

                    a.position = glm::normalize(a.position);
                    if(j == -1 || i == -1 || j == size + 1 || i == size +1)
                        a.position *= 0.99;

                    a.normal = a.position;

                    a.uv = {i * (1.0/(float)size), j * (1.0/(float)size) };

                    a.uv_glob = {xs + i * dd + 0.5f, ys + j * dd + 0.5f};

                    terminal_mesh->Vertices.push_back(a);
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

            terminal_mesh->Indices = parent->Indeces;
            terminal_mesh->ForgetBind();
            //terminal_mesh->BindExistingIBO(parent->ibo, parent->Indeces.size());
            status = READY;
            terminal_mesh->Render(cam, parent->world);
        }
    }
    else
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i]->Render(cam, side, parent);
    }

}

#undef T_L_D_R_M_STAR

void QuadPlane::Update(Camera &camera, float Rs, float eps, int max_divide, SphereParamsStorage *parent)
{
    glm::vec3 cent[4];
    for(int i = 0; i < 4; i++)
    {
        cent[i] = glm::vec3(parent->world * glm::vec4(subsurface_centers[i], 0));
    }

    if(is_terminal() &&
       (glm::distance(cent[0] * Rs, camera.Position()) < eps * scale ||
        glm::distance(cent[1] * Rs, camera.Position()) < eps * scale ||
        glm::distance(cent[2] * Rs, camera.Position()) < eps * scale ||
        glm::distance(cent[3] * Rs, camera.Position()) < eps * scale)
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
        if((glm::distance(cent[0] * Rs, camera.Position()) > eps * 1.1f * scale &&
            glm::distance(cent[1] * Rs, camera.Position()) > eps * 1.1f * scale &&
            glm::distance(cent[2] * Rs, camera.Position()) > eps * 1.1f * scale &&
            glm::distance(cent[3] * Rs, camera.Position()) > eps * 1.1f * scale))
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i] = nullptr;
    }

    if(!is_terminal())
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i]->Update(camera, Rs, eps, max_divide, parent);
    }
}

