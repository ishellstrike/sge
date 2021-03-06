#include "sector.h"
#include "db.h"
#include "textureatlas.h"
#include "colorextender.h"
#include "prefecences.h"
#include "core/agents/entity.h"
#include "core/agents/agents.hpp"

Sector::Sector(const glm::ivec2 &o) : offset(o)
{
//    for(int i = 0; i < RX; i++)
//        for(int j = 0; j < RY; j++)
//            for(int k = 0; k < RZ; k++)
//            {
//                data[ONEDIM(i,j,k)] = DB::Create("air");
    //            }
}

Sector::Sector()
{

}

void Sector::Update(Level *l, GameTimer& gt)
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
            for(int k = 0; k < maxlevel + 1; k++)
            {
                std::shared_ptr<Object> &o = block[ONEDIM(i,j,k)];
                static const ObjectStatic *air = DB::Get("air");
                if(o->base->id != air->id)
                    o->onUpdate(o, l, {i,j,k}, gt);
            }
}

#ifdef CLIENT
std::array<glm::vec3, 4> GetAtBlockPoints(float xpos, float ypos, int resx, int resy)
{
   float vix = (sscale - resx) / 2.0f;
   float viy = (sscale - resy) / 2.0f;

   auto p1 = glm::vec3(xpos + vix, ypos + vix, 0);
   auto p2 = glm::vec3(xpos + resx + vix, ypos + viy, 0);
   auto p3 = glm::vec3(xpos + resx + vix, ypos + resy + viy, 0);
   auto p4 = glm::vec3(xpos + vix, ypos + resy + viy, 0);

   return { p1, p2, p3, p4 };
}

void DrawShadowForBlock(SpriteBatch &sb, const glm::vec3 &per, const glm::vec2 &off, float xpos, float ypos)
{
    std::array<glm::vec3, 4> po = GetAtBlockPoints(xpos, ypos, int(sscale), int(sscale));

    auto car = glm::vec3(per.x * sscale - off.x, per.y * sscale - off.y, 0);

    //лучи ко всем вершинам блока
    sge::ray r1(car, po[0] - car);
    sge::ray r2(car, po[1] - car);
    sge::ray r3(car, po[2] - car);
    sge::ray r4(car, po[3] - car);

    std::array<glm::vec3, 4> po2 = {
        r1.origin + r1.dir * 1500.f, r2.origin + r2.dir * 1500.f, r3.origin + r3.dir * 1500.f, r4.origin + r4.dir * 1500.f
    };

    std::array<int, 2> spoints = {0,0};

    //Выбор крайних точек блока
    if (car.x <= xpos)
    {
        //left column
        spoints = car.y <= ypos ? std::array<int, 2>{1, 3}
                                : car.y <= ypos + sscale ? std::array<int, 2>{0, 3}
                                                         : std::array<int, 2>{0, 2};
    }
    else if (car.x <= xpos + sscale)
    {
        //middle column
        spoints = car.y <= ypos ? std::array<int, 2>{1, 0}
                                : car.y <= ypos + sscale ? std::array<int, 2>{0, 0}
                                                         : std::array<int, 2>{3, 2};
    }
    else
    {
        //right column
        spoints = car.y <= ypos ? std::array<int, 2>{2, 0}
                                : car.y <= ypos + sscale ? std::array<int, 2>{2, 1}
                                                         : std::array<int, 2>{3, 1};
    }

    //Making shadow polys
    sb.drawTriangle(glm::vec2(po2[spoints[0]]), glm::vec2(po[spoints[0]]), glm::vec2(po2[spoints[1]]), Color::Black);
    sb.drawTriangle(glm::vec2(po[spoints[0]]), glm::vec2(po[spoints[1]]), glm::vec2(po2[spoints[1]]), Color::Black);
}

auto atlas_draw = [&](int x, int y, const Object &b, SpriteBatch &sb){
    const ObjectStatic &o = *b.base;
    if(o.id == "air" || o.tex.size() == 0)
        return;

    auto btex = b.otex;
    auto otex = o.tex[btex];
    auto t = TextureAtlas::refs[otex];
    sb.drawQuadAtlas({x, y}, {sscale, sscale}, TextureAtlas::tex[t.x], t.y, Color::White);
};

void Sector::DrawBlock(SpriteBatch &sb, const glm::ivec2 &off, const glm::vec3 &) const
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
        {
            int x = int((i + offset.x*RX)*sscale - off.x);
            int y = int((j + offset.y*RY)*sscale - off.y);
            if(x > RESX || y > RESY)
                continue;
            if(x + sscale < 0 || y + sscale < 0)
                continue;
            for(int k = 0; k < maxlevel + 1; k++)
            {
                const Object &b2 = *block[ONEDIM(i,j,k)];
                atlas_draw(x, y, b2, sb);
            }
        }
}

void Sector::DrawShadow(SpriteBatch &sb, const glm::ivec2 &off, const glm::vec3 &hpos) const
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
        {
            int x = int((i + offset.x*RX)*sscale - off.x);
            int y = int((j + offset.y*RY)*sscale - off.y);
            if(x > RESX || y > RESY)
                continue;
            if(x + sscale < 0 || y + sscale < 0)
                continue;

            for(int k = 0; k < 1; k++)
            {
                const Object &b2 = *block[ONEDIM(i,j,k)];
                static void *air_ref = DB::data["air"].get();
                if(b2.base != air_ref && !b2.base->HasAgent<Transparent>())
                    DrawShadowForBlock(sb, hpos, off, x, y);
            }
        }
}

void Sector::Draw(SpriteBatch &sb, const glm::ivec2 &off, const glm::vec3 &) const
{

    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
        {
            int x = int((i + offset.x*RX)*sscale - off.x);
            int y = int((j + offset.y*RY)*sscale - off.y);
            if(x > RESX || y > RESY)
                continue;
            if(x + sscale < 0 || y + sscale < 0)
                continue;
            for(int k = 0; k < 1; k++)
            {
                const Object &b2 = *ground[ONEDIM(i,j,k)];
                atlas_draw(x, y, b2, sb);
            }
        }
}

void Sector::DrawEntities(SpriteBatch &sb, const glm::ivec2 &off, const glm::vec3 &) const
{
    for(const std::shared_ptr<Object> &a : entities)
    {
        if(const Creature *e = a->GetAgent<Creature>())
        {
            atlas_draw(int(e->pos.x*sscale - off.x - 16), int(e->pos.y*sscale - off.y - sscale), *a, sb);
        }
    }
}
#endif

void Sector::SetBlock(const glm::ivec3 &pos, std::shared_ptr<Object> obj)
{
    static void *air_ref = DB::data["air"].get();
    if(obj->base != air_ref)
        maxlevel = pos.z;
    block[ONEDIM( pos.x, pos.y, pos.z )] = obj;
}

void Sector::SetGround(const glm::ivec3 &pos, std::shared_ptr<Object> obj)
{
    static void *air_ref = DB::data["air"].get();
    if(obj->base != air_ref)
        maxlevel = pos.z;
    ground[ONEDIM( pos.x, pos.y, pos.z )] = obj;
}

std::pair<std::shared_ptr<Object>, std::shared_ptr<Object>>Sector::GetCell( const glm::ivec3 &pos )
{
    if( pos.x >= RX || pos.y >= RY || pos.x < 0 || pos.y < 0)
        return std::make_pair(std::shared_ptr<Object>(), std::shared_ptr<Object>());
    return std::make_pair(block[ ONEDIM( pos.x, pos.y, pos.z ) ], ground[ ONEDIM( pos.x, pos.y, pos.z ) ]);
}

std::shared_ptr<Object> Sector::GetBlock( const glm::ivec3 &pos )
{
    if( pos.x >= RX || pos.y >= RY || pos.x < 0 || pos.y < 0)
        return std::shared_ptr<Object>();
    return block[ ONEDIM( pos.x, pos.y, pos.z ) ];
}

std::shared_ptr<Object> Sector::GetGround( const glm::ivec3 &pos )
{
    if( pos.x >= RX || pos.y >= RY || pos.x < 0 || pos.y < 0)
        return std::shared_ptr<Object>();
    return ground[ ONEDIM( pos.x, pos.y, pos.z ) ];
}

void Sector::RebuildMax()
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
            for(int k = 0; k < RZ; k++)
            {
                if(block[ONEDIM(i,j,k)]->base != DB::data["air"].get())
                    maxlevel = glm::max(maxlevel, k);
            }
}

void Sector::PlaceScheme(const Scheme &s, const glm::ivec3 &pos)
{
    for(int i = 0; i < s.size.x && pos.x + i < RX; i++)
    {
        for(int j = 0; j < s.size.y && pos.y + j < RY; j++)
        {
            Letter l = s.data[j][i];
            if(l == '.') continue;
            auto dict = s.dict.find(l);
            block[ONEDIM(pos.x + i, pos.y + j, pos.z)] = DB::Create(dict->second);
        }
    }
}

