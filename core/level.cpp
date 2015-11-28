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
    std::vector<Sector*> t;
    for(const auto &i : map)
        t.push_back(i.second.get());
    std::sort(std::begin(t), std::end(t), [](Sector* a, Sector* b)
    {
        return a->offset.x == b->offset.x ?
               a->offset.y > b->offset.y  :
               a->offset.x > b->offset.x  ;
    });

    for(const auto &i : t)
        i->Draw(sb, off);
}

