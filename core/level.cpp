#include "level.h"
#include "prefecences.h"

Level::Level()
{

}

Sector *Level::GetSector(const glm::vec2 &off)
{
    auto t = map.find(off);
    if(t != map.end())
        return (*t).second.get();

    map[off] = std::unique_ptr<Sector>(new Sector(off));
    Sector *s = map[off].get();
    s->Generate();

    return s;
}

void Level::Update()
{

}

void Level::Draw(SpriteBatch &sb) const
{
    for(const auto &i : map)
    {
        auto max = (i.second->offset + glm::ivec2(1)) * glm::ivec2(RX, RY) + off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = i.second->offset * glm::ivec2(RX, RY) + off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->Draw(sb, off);
    }
}

