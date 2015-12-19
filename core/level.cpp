#include "level.h"
#include "prefecences.h"
#include "remoteclient.h"
#include <thread>

Level::Level()
{

}

Sector *Level::GetSectorByPos(const glm::vec3 &p)
{
    int x = p.x < 0 ? (int(p.x) + 1) / RX - 1 : int(p.x) / RX;
    int y = p.y < 0 ? (int(p.y) + 1) / RY - 1 : int(p.y) / RY;

    return GetSector({x,y});
}

Sector *Level::GetSector(const glm::ivec2 &off)
{
    auto t = map.find(off);
    if(t != map.end())
        return (*t).second.get();

    auto s = RemoteClient::instance().GetSector( off );
    auto sr = s.get();
    if(sr != nullptr)
        map.insert( std::make_pair( off, std::move( s ) ) );
    return sr;
}

void Level::Update()
{
    for(const auto &i : map)
    {
        Sector &cur = *i.second;
        for(const std::shared_ptr<Object> &a : cur.entities)
        {

        }

        for(auto c_iter = std::begin(cur.entities); c_iter != std::end(cur.entities);)
        {
            Object &c = **c_iter;
            glm::vec3 pos = c.GetAgent<Entity>()->pos;

            // если находится за границей сектора -- переносим в новый сектор, если он существует
            if(pos.x > (cur.offset.x + 1) * RX ||
               pos.x < (cur.offset.x    ) * RX ||
               pos.y > (cur.offset.y + 1) * RY ||
               pos.y < (cur.offset.y    ) * RY )
            {
                if(Sector *nsec = GetSectorByPos(pos))
                {
                    nsec->entities.insert(std::end(nsec->entities), std::move(*c_iter));
                    cur.entities.erase(c_iter);
                    break;
                }
            }
            //передвигаем итератор, если существо не перемещено
            ++c_iter;
        }
    }
}

void Level::Draw(SpriteBatch &sb, const glm::vec2 &off) const
{
    for(const auto &i : map)
    {
        auto max = (glm::vec2(i.second->offset) + glm::vec2(1.f, 1.f)) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = glm::vec2(i.second->offset) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->Draw(sb, off);
    }
}

bool Level::AddEntity( std::shared_ptr<Object> o, bool wait )
{
    if( const Entity *e = o->GetAgent<Entity>() )
    {
        Sector *s;
        while( !(s = GetSectorByPos(e->pos)) && wait )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
            s = GetSectorByPos(e->pos);
        }

        if( s != nullptr )
        {
            s->entities.insert( s->entities.end(), std::move( o ) );
            return true;
        }
        return false;
    }
    else
        LOG(error) << "not an entity";
}

