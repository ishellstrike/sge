#include "level.h"

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
        i.second->Draw(sb, off);
}

