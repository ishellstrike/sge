#include "sector.h"
#include "db.h"
#include "textureatlas.h"
#include "colorextender.h"

Sector::Sector(const glm::vec2 &o) : offset(o)
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

void Sector::Draw(SpriteBatch &sb, const glm::vec2 &off) const
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
            for(int k = 0; k < maxlevel + 1; k++)
            {
                int x = (i + offset.x*RX)*32 - off.x + 200;
                int y = (j + offset.y*RY)*32 - off.y + 200;

                const Object &b = *data[ONEDIM(i,j,k)];
                const ObjectStaticHelper &o = *b.base;
                if(o.id == "air")
                    continue;
                auto t = TextureAtlas::refs[o.tex[b.otex]];
                sb.drawQuadAtlas({x, y}, {32,32}, TextureAtlas::tex[t.x], t.y, Color::White);
            }
}

void Sector::SetObject(const glm::ivec3 &pos, std::unique_ptr<Object> obj)
{
    if(obj->base != DB::data["air"].get())
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

void Sector::PlaceScheme(const Scheme &s, glm::vec3 pos)
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

