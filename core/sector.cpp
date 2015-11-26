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
            data[ONEDIM(k,j,0)] = DB::Create("dirt");
        }

    RebuildMax();
}

void Sector::Update()
{

}

void Sector::Draw(SpriteBatch &sb, const glm::vec2 &off) const
{
    for(int i = RX - 1; i >= 0; i--)
        for(int j = 0; j < RY; j++)
            for(int k = 0; k < maxlevel + 1; k++)
            {
                int x = (i + offset.x*RX)*64/2 - (j + offset.y*RY)*64/2 - k*32 + off.x + 200;
                int y = (i + offset.x*RX)*32/2 + (j + offset.y*RY)*32/2 - k*32 + off.y + 200;

                ObjectStaticHelper *o = data[ONEDIM(i,j,k)]->base;
                if(o->id == "air")
                    continue;
                auto t = TextureAtlas::refs[o->tex];
                sb.drawQuadAtlasJARG({x, y}, 1, TextureAtlas::tex[t.x], t.y, Color::White);
            }
}

void Sector::SetObject(const glm::ivec3 &pos, std::unique_ptr<Object> obj)
{
    if(obj->base != ObjectStatic::air.get())
        maxlevel = pos.z;
}

void Sector::RebuildMax()
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
            for(int k = 0; k < RZ; k++)
            {
                if(data[ONEDIM(i,j,k)]->base != ObjectStatic::air.get())
                    maxlevel = glm::max(maxlevel, k);
            }
}

