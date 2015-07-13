#include "quadplane.h"
#include <glm/gtx/transform.hpp>
#include "ClassicNoise.h"

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
    return m_parts[0] == nullptr;
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

void QuadPlane::Render(const glm::mat4 &MVP, std::shared_ptr<Material> &mat, std::shared_ptr<BasicJargShader> &basic, int side)
{
    if(is_terminal())
    {
        if(status == READY)
            terminal_mesh->Render(MVP);
        else
        {
            terminal_mesh->material = mat;
            terminal_mesh->shader = basic;

            const int size = 32;
            terminal_mesh->Indices.reserve(size * size * 6);
            terminal_mesh->Vertices.reserve((size + 1) * (size + 1));
            int co = 0;

            float xs = (-0.5 + offset.x); /*< x координата начала сектора сферы с отступом*/
            float ys = (-0.5 + offset.y); /*< y координата начала сектора сферы с отступом*/
            float dd = ((1.0*scale)/((float)size-2)); /*< размер сектора сферы*/

            //Генерация R=1 сферы. Нормализуемые плоскости имеют координаты [-0.5, 0.5]. В шейдере свера приводится к радиусу R

            for(int j = 0; j < size + 1; j++)
            {
                for(int i = 0; i < size + 1; i++)
                {
                    VertPosNormTanBiTex a;
                    a.position = {xs + i * dd, ys + j * dd, 0.5f};

                    a.position = glm::normalize(a.position);

                    a.normal = a.position;

                    a.uv = {xs + i * dd + 0.5f, ys + j * dd + 0.5f};

                    terminal_mesh->Vertices.push_back(a);
                }
            }

            std::vector<QuadPlane *> nighbours = {0,0,0,0};//getRoute();

            if(!nighbours[TOP_N] || nighbours[TOP_N]->level >= level)
            {
                for(int i = 0; i < size - 1; ++i)
                {
                    int j = 0;
                    int tl = (j)*(size + 1) +   i;
                    int tr = (j)*(size + 1) +   i + 1;
                    int dl = (j+1)*(size + 1) + i;
                    int dr = (j+1)*(size + 1) + i + 1;

                    PUSH_BACKWARD
                }
            }
            else
            {
                terminal_mesh->Indices.push_back(0);
                terminal_mesh->Indices.push_back(2);
                terminal_mesh->Indices.push_back((size+1)+1);

                terminal_mesh->Indices.push_back(2);
                terminal_mesh->Indices.push_back((size+1)+2);
                terminal_mesh->Indices.push_back((size+1)+1);
                for(int i = 2; i < size - 2; i+=2)
                {
                    int j = 0;
                    int tl =  (j)  *(size + 1) + i;
                    int tr =  (j)  *(size + 1) + i + 2;
                    int dl =  (j+1)*(size + 1) + i;
                    int dr =  (j+1)*(size + 1) + i + 2;
                    int mid = (j+1)*(size + 1) + i + 1;
                    //  ____________________
                    //  \0|\ 3/|\  /|\  /|x/
                    //   \|1\/2|_\/_|_\/_|/

                    terminal_mesh->Indices.push_back(tl); //1
                    terminal_mesh->Indices.push_back(mid);
                    terminal_mesh->Indices.push_back(dl);

                    terminal_mesh->Indices.push_back(tr); //2
                    terminal_mesh->Indices.push_back(dr);
                    terminal_mesh->Indices.push_back(mid);

                    terminal_mesh->Indices.push_back(tl); //3
                    terminal_mesh->Indices.push_back(tr);
                    terminal_mesh->Indices.push_back(mid);
                }
                terminal_mesh->Indices.push_back(size-2);
                terminal_mesh->Indices.push_back(size);
                terminal_mesh->Indices.push_back((size+1)+size-1);

                terminal_mesh->Indices.push_back(size-2);
                terminal_mesh->Indices.push_back((size+1)+size-1);
                terminal_mesh->Indices.push_back((size+1)+size-2);
            }

            if(!nighbours[RIGHT_N] || nighbours[RIGHT_N]->level >= level)
            {
                for(int j = 0; j < size - 1; ++j)
                {
                    int i = size-1;
                    int tl = (j)*(size + 1) +   i;
                    int tr = (j)*(size + 1) +   i + 1;
                    int dl = (j+1)*(size + 1) + i;
                    int dr = (j+1)*(size + 1) + i + 1;

                    PUSH_BACKWARD
                }
            }
            else
            {
                terminal_mesh->Indices.push_back((0)*(size+1)+size); //x
                terminal_mesh->Indices.push_back((2)*(size+1)+size);
                terminal_mesh->Indices.push_back((1)*(size+1)+size-1);

                terminal_mesh->Indices.push_back((1)*(size+1)+size-1); //x
                terminal_mesh->Indices.push_back((2)*(size+1)+size);
                terminal_mesh->Indices.push_back((2)*(size+1)+size-1);

                for(int j = 2; j < size - 2; j+=2)
                {
                    int i = size-1;
                    int tl =  (j)  *(size + 1) + i;
                    int tr =  (j)  *(size + 1) + i + 1;
                    int dl =  (j+2)*(size + 1) + i;
                    int dr =  (j+2)*(size + 1) + i + 1;
                    int mid_right = (j+1)*(size + 1) + i;
                    //  1/|
                    //  / |
                    //  \3|
                    //  2\|

                    terminal_mesh->Indices.push_back(tl);
                    terminal_mesh->Indices.push_back(tr);
                    terminal_mesh->Indices.push_back(mid_right);

                    terminal_mesh->Indices.push_back(tr);
                    terminal_mesh->Indices.push_back(dr);
                    terminal_mesh->Indices.push_back(mid_right);

                    terminal_mesh->Indices.push_back(mid_right);
                    terminal_mesh->Indices.push_back(dr);
                    terminal_mesh->Indices.push_back(dl);
                }

                terminal_mesh->Indices.push_back((size-2)*(size+1)+size-1); //x
                terminal_mesh->Indices.push_back((size-2)*(size+1)+size);
                terminal_mesh->Indices.push_back((size-1)*(size+1)+size-1);

                terminal_mesh->Indices.push_back((size-1)*(size+1)+size-1); //x
                terminal_mesh->Indices.push_back((size-2)*(size+1)+size);
                terminal_mesh->Indices.push_back((size)*(size+1)+size);
            }

            if(!nighbours[LEFT_N] || nighbours[LEFT_N]->level >= level)
            {
                for(int j = 0; j < size - 1; ++j)
                {
                    int i = 0;
                    int tl = (j)*(size + 1) +   i;
                    int tr = (j)*(size + 1) +   i + 1;
                    int dl = (j+1)*(size + 1) + i;
                    int dr = (j+1)*(size + 1) + i + 1;

                    PUSH_BACKWARD
                }
            }
            else
            {
                terminal_mesh->Indices.push_back(0);
                terminal_mesh->Indices.push_back(size+1+1);
                terminal_mesh->Indices.push_back((size+1)*2);

                terminal_mesh->Indices.push_back(size+1+1);
                terminal_mesh->Indices.push_back((size+1)*2+1);
                terminal_mesh->Indices.push_back((size+1)*2);
                for(int j = 2; j < size - 2; j+=2)
                {
                    int i = 0;
                    int tl =  (j)  *(size + 1) + i;
                    int tr =  (j)  *(size + 1) + i + 1;
                    int dl =  (j+2)*(size + 1) + i;
                    int dr =  (j+2)*(size + 1) + i + 1;
                    int mid_right = (j+1)*(size + 1) + i + 1;
                    //  |\1
                    //  |3\
                    //  | /
                    //  |/2

                    terminal_mesh->Indices.push_back(tl);
                    terminal_mesh->Indices.push_back(tr);
                    terminal_mesh->Indices.push_back(mid_right);

                    terminal_mesh->Indices.push_back(mid_right);
                    terminal_mesh->Indices.push_back(dr);
                    terminal_mesh->Indices.push_back(dl);

                    terminal_mesh->Indices.push_back(tl);
                    terminal_mesh->Indices.push_back(mid_right);
                    terminal_mesh->Indices.push_back(dl);
                }
                terminal_mesh->Indices.push_back((size-2)*(size+1));
                terminal_mesh->Indices.push_back((size-2)*(size+1)+1);
                terminal_mesh->Indices.push_back((size-1)*(size+1)+1);

                terminal_mesh->Indices.push_back((size-2)*(size+1));
                terminal_mesh->Indices.push_back((size-1)*(size+1)+1);
                terminal_mesh->Indices.push_back((size)*(size+1));
            }

            if(!nighbours[BOTTOM_N] || nighbours[BOTTOM_N]->level >= level)
            {
                for(int i = 0; i < size - 1; ++i)
                {
                    int j = size - 1;
                    int tl = (j)*(size + 1) +   i;
                    int tr = (j)*(size + 1) +   i + 1;
                    int dl = (j+1)*(size + 1) + i;
                    int dr = (j+1)*(size + 1) + i + 1;

                    PUSH_BACKWARD
                }
            }
            else
            {
                terminal_mesh->Indices.push_back((size-1)*(size+1)+1); //0
                terminal_mesh->Indices.push_back((size)*(size+1)+2);
                terminal_mesh->Indices.push_back((size)*(size+1));

                terminal_mesh->Indices.push_back((size-1)*(size+1)+1);
                terminal_mesh->Indices.push_back((size-1)*(size+1)+2);
                terminal_mesh->Indices.push_back((size)*(size+1)+2);
                for(int i = 2; i < size - 2; i+=2)
                {
                    int j = size - 1;
                    int tl =      (j)  *(size + 1) + i;
                    int tr =      (j)  *(size + 1) + i + 2;
                    int dl =      (j+1)*(size + 1) + i;
                    int dr =      (j+1)*(size + 1) + i + 2;
                    int mid_top = (j)*(size + 1) + i + 1;
                    //  __________________________
                    //  \0|1/\2|| /\ | /\ | /\ |x/
                    //   \|/_3\||/__\|/__\|/__\|/

                    terminal_mesh->Indices.push_back(tl);
                    terminal_mesh->Indices.push_back(mid_top);
                    terminal_mesh->Indices.push_back(dl);

                    terminal_mesh->Indices.push_back(mid_top);
                    terminal_mesh->Indices.push_back(tr);
                    terminal_mesh->Indices.push_back(dr);

                    terminal_mesh->Indices.push_back(dl);
                    terminal_mesh->Indices.push_back(mid_top);
                    terminal_mesh->Indices.push_back(dr);
                }
                terminal_mesh->Indices.push_back((size-1)*(size+1)+size-2); //x
                terminal_mesh->Indices.push_back((size-1)*(size+1)+size-1);
                terminal_mesh->Indices.push_back((size-0)*(size+1)+size-2);

                terminal_mesh->Indices.push_back((size-1)*(size+1)+size-1); //x
                terminal_mesh->Indices.push_back((size)*(size+1)+size);
                terminal_mesh->Indices.push_back((size)*(size+1)+size-2);
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
            terminal_mesh->Render(MVP);
        }
    }
    else
    {
        for(int i = 0; i < 4; ++i)
            m_parts[i]->Render(MVP, mat, basic, side);
    }
}

#undef T_L_D_R_M_STAR
#undef PUSH_BACKWARD
#undef PUSH_FORWARD

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
    }
    else
        if((glm::distance(subsurface_centers[0] * Rs, camera.getPosition()) > eps * 1.1f * scale &&
            glm::distance(subsurface_centers[1] * Rs, camera.getPosition()) > eps * 1.1f * scale &&
            glm::distance(subsurface_centers[2] * Rs, camera.getPosition()) > eps * 1.1f * scale &&
            glm::distance(subsurface_centers[3] * Rs, camera.getPosition()) > eps * 1.1f * scale))
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

