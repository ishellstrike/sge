#include "sector.h"
#include "db.h"
#include "textureatlas.h"
#include "colorextender.h"
#include "prefecences.h"
#include "core/agents/entity.h"

Sector::Sector(const glm::ivec2 &o) : offset(o)
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
            for(int k = 0; k < RZ; k++)
            {
                data[ONEDIM(i,j,k)] = DB::Create("air");
            }
}

void Sector::Generate()
{
    for(int j = 0; j < RY; j++)
        for(int k = 0; k < RZ; k++)
        {
            data[ONEDIM(k,j,0)] = DB::Create("grass");
        }

    PlaceScheme(DB::scheme_db["house1_room"][0], {0,0,1});

    RebuildMax();
}

void Sector::Update()
{
}

void Sector::Draw(SpriteBatch &sb, const glm::ivec2 &off) const
{
    auto atlas_draw = [&](int x, int y, const Object &b){
        const ObjectStaticHelper &o = *b.base;
        if(o.id == "air" || o.tex.size() == 0)
            return;

        auto btex = b.otex;
        auto otex = o.tex[btex];
        auto t = TextureAtlas::refs[otex];
        sb.drawQuadAtlas({x, y}, {32,32}, TextureAtlas::tex[t.x], t.y, Color::White);
    };

    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
        {
            int x = (i + offset.x*RX)*32 - off.x;
            int y = (j + offset.y*RY)*32 - off.y;
            if(x > RESX || y > RESY)
                continue;
            if(x + 32 < 0 || y + 32 < 0)
                continue;
            for(int k = 0; k < maxlevel + 1; k++)
            {
                const Object &b = *data[ONEDIM(i,j,k)];
                atlas_draw(x, y, b);
            }
        }

    for(const std::unique_ptr<Object> &a : entities)
    {
        if(const Entity *e = a->GetAgent<Entity>())
        {
            atlas_draw(offset.x*RX*32 + e->pos.x*32.f - off.x, offset.y*RY*32 + e->pos.y*32.f - off.y, *a);
        }
    }
}

void Sector::SetObject(const glm::ivec3 &pos, std::unique_ptr<Object> obj)
{
    static void *air_ref = DB::data["air"].get();
    if(obj->base != air_ref)
        maxlevel = pos.z;
}

void Sector::RebuildMax()
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
            for(int k = 0; k < RZ; k++)
            {
                if(data[ONEDIM(i,j,k)]->base != DB::data["air"].get())
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
            data[ONEDIM(pos.x + i, pos.y + j, pos.z)] = DB::Create(dict->second);
        }
    }
}

