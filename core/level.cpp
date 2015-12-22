#include "level.h"
#include "prefecences.h"
#include "remoteclient.h"
#include <thread>
#include "db.h"

Level::Level()
{

}

Object *Level::GetObjectByPos(const glm::vec3 &pos)
{
    int sec_x = pos.x < 0 ? (int(pos.x) + 1) / RX - 1 : int(pos.x) / RX;
    int sec_y = pos.y < 0 ? (int(pos.y) + 1) / RY - 1 : int(pos.y) / RY;
    auto sec_pos = pos - glm::vec3(sec_x*RX, sec_y*RY, 0);

    if( Sector* s = GetSector({sec_x,sec_y}) )
    {
        return s->GetObject(sec_pos);
    }
    return nullptr;
}

bool Level::SetObjectAtPos(const glm::vec3 &pos, std::shared_ptr<Object> o)
{
    int sec_x = pos.x < 0 ? (int(pos.x) + 1) / RX - 1 : int(pos.x) / RX;
    int sec_y = pos.y < 0 ? (int(pos.y) + 1) / RY - 1 : int(pos.y) / RY;
    auto sec_pos = pos - glm::vec3(sec_x*RX, sec_y*RY, 0);

    if( Sector* s = GetSector({sec_x,sec_y}) )
    {
        s->SetObject(sec_pos, o);
        return true;
    }
    return nullptr;
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

bool Level::TpEntity( Object &o, const glm::vec3 &pos, bool wait )
{
    Sector *s;
    while( !(s = GetSectorByPos( pos )) && wait )
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    if( !s )
        return false;

    int sec_x = pos.x < 0 ? (int(pos.x) + 1) / RX - 1 : int(pos.x) / RX;
    int sec_y = pos.y < 0 ? (int(pos.y) + 1) / RY - 1 : int(pos.y) / RY;
    auto sec_pos = pos - glm::vec3(sec_x*RX, sec_y*RY, 0);
    if(Object *targ = s->GetObject( sec_pos ))
    {
        if( auto w = static_cast<ObjectStatic*>(targ->base)->GetAgent<Walkable>() )
            o.GetAgent<Entity>()->pos = pos;
    }

    return false;
}

bool Level::DeltaEntity(Object &o, const glm::vec3 &delta, bool wait )
{
    auto pos = o.GetAgent<Entity>()->pos;
    auto new_pos = pos + delta;
    Sector *s;
    while( !(s = GetSectorByPos( new_pos )) && wait )
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    if( !s )
        return false;

    if(int(pos.x) != int(new_pos.x) ||
       int(pos.y) != int(new_pos.y) ||
       int(pos.z) != int(new_pos.z))
    {
        if(Object *o = GetObjectByPos(pos))
        {
            o->onLeave(pos, GameTimer());
        }
        if(Object *o = GetObjectByPos(new_pos))
        {
            o->onEnter(new_pos, GameTimer());
        }
    }

    int sec_x = new_pos.x < 0 ? (int(new_pos.x) + 1) / RX - 1 : int(new_pos.x) / RX;
    int sec_y = new_pos.y < 0 ? (int(new_pos.y) + 1) / RY - 1 : int(new_pos.y) / RY;
    auto sec_pos = new_pos - glm::vec3( sec_x*RX, sec_y*RY, 0 );
    std::pair<Object *, Object *> targ = s->GetCell( sec_pos );
    if( targ.first )
    {
        static const ObjectStatic *air = DB::Get("air");

        Walkable *w = targ.first->base->GetAgent<Walkable>();
        if( w && targ.second->base != air )
            o.GetAgent<Entity>()->pos = pos + delta / w->cost;
    }

    return false;
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
            if(pos.x >= (cur.offset.x + 1) * RX ||
               pos.x < (cur.offset.x    ) * RX ||
               pos.y >= (cur.offset.y + 1) * RY ||
               pos.y < (cur.offset.y    ) * RY )
            {
                if(Sector *nsec = GetSectorByPos(pos))
                {
                    nsec->entities.insert(std::end(nsec->entities), std::move(*c_iter));
                    //nsec->SetObject(pos, DB::Create("briwall"));
                    cur.entities.erase(c_iter);
                    break;
                }
            }
            //передвигаем итератор, если существо не перемещено
            ++c_iter;
        }
    }
}

void Level::Draw(SpriteBatch &sb, const glm::vec2 &off, glm::vec3 &hpos) const
{
    for(const auto &i : map)
    {
        auto max = (glm::vec2(i.second->offset) + glm::vec2(1.f, 1.f)) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = glm::vec2(i.second->offset) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->Draw(sb, off, hpos);
    }

    for(const auto &i : map)
    {
        auto max = (glm::vec2(i.second->offset) + glm::vec2(1.f, 1.f)) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = glm::vec2(i.second->offset) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->DrawShadow(sb, off, hpos);
    }

    for(const auto &i : map)
    {
        auto max = (glm::vec2(i.second->offset) + glm::vec2(1.f, 1.f)) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = glm::vec2(i.second->offset) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->DrawBlock(sb, off, hpos);
    }

    for(const auto &i : map)
    {
        auto max = (glm::vec2(i.second->offset) + glm::vec2(1.f, 1.f)) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = glm::vec2(i.second->offset) * glm::vec2(RX*32.f, RY*32.f) - off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->DrawEntities(sb, off, hpos);
    }
}

bool Level::AddEntity( std::shared_ptr<Object> o, bool wait )
{
    if( const Entity *e = o->GetAgent<Entity>() )
    {
        Sector *s;

        while( !(s = GetSectorByPos(e->pos)) && wait )
            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );

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

