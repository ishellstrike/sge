#include "level.h"
#include "prefecences.h"
#include "remoteclient.h"
#include <thread>
#include "db.h"

Level::Level()
{

}

std::shared_ptr<Object> Level::GetObjectByPos(const glm::vec3 &pos)
{
    int sec_x = pos.x < 0 ? (int(pos.x) + 1) / RX - 1 : int(pos.x) / RX;
    int sec_y = pos.y < 0 ? (int(pos.y) + 1) / RY - 1 : int(pos.y) / RY;
    auto sec_pos = pos - glm::vec3(sec_x*RX, sec_y*RY, 0);

    if( Sector* s = GetSector({sec_x,sec_y}) )
    {
        return s->GetBlock(sec_pos);
    }
    return nullptr;
}

std::list<Sector *> Level::GetSectorsInRange(const glm::vec3 &pos, float r)
{
    int d = ((r+1))/RX;
    std::list<Sector *> s;

    int sec_x = pos.x < 0 ? (int(pos.x) + 1) / RX - 1 : int(pos.x) / RX;
    int sec_y = pos.y < 0 ? (int(pos.y) + 1) / RY - 1 : int(pos.y) / RY;
    glm::ivec2 sec_pos(sec_x, sec_y);

    Sector *s1 = GetSector(sec_pos, false);
    if(!s1)
        return std::list<Sector *>();
    s.insert(s.begin(), s1);
    for(int i = -d; i <= d; ++i)
        for(int j = -d; j <= d; ++j)
            if( i != 0 && j != 0)
            {
                glm::vec3 p1((sec_pos.x + i)*RX, (sec_pos.y + j)*RY, 0);
                if(glm::distance(pos, p1) <= r)
                {
                    Sector *s2 = GetSector(sec_pos + glm::ivec2(i, j), false);
                    if(s2)
                        s.insert(s.begin(), s2);
                }
            }
    return s;
}

std::list<std::weak_ptr<Object>> Level::GetObjectsInRange(const glm::vec3 &coord, float r)
{
    std::list<Sector *> s = GetSectorsInRange(coord, r);
    std::list<std::weak_ptr<Object>> ob;
    for(Sector *i : s)
    {
        for(std::shared_ptr<Object> &c : i->entities)
        {
            glm::vec3 p = c->GetAgent<Creature>()->pos;
            if(glm::distance(coord, p) <= r)
                ob.insert(ob.begin(), std::weak_ptr<Object>(c));
        }
    }
    return ob;
}

bool Level::SetBlockAtPos(const glm::vec3 &pos, std::shared_ptr<Object> o)
{
    int sec_x = pos.x < 0 ? (int(pos.x) + 1) / RX - 1 : int(pos.x) / RX;
    int sec_y = pos.y < 0 ? (int(pos.y) + 1) / RY - 1 : int(pos.y) / RY;
    auto sec_pos = pos - glm::vec3(sec_x*RX, sec_y*RY, 0);

    if( Sector* s = GetSector({sec_x,sec_y}) )
    {
        s->SetBlock(sec_pos, o);
        return true;
    }
    return nullptr;
}

bool Level::SetGroundAtPos(const glm::vec3 &pos, std::shared_ptr<Object> o)
{
    int sec_x = pos.x < 0 ? (int(pos.x) + 1) / RX - 1 : int(pos.x) / RX;
    int sec_y = pos.y < 0 ? (int(pos.y) + 1) / RY - 1 : int(pos.y) / RY;
    auto sec_pos = pos - glm::vec3(sec_x*RX, sec_y*RY, 0);

    if( Sector* s = GetSector({sec_x,sec_y}) )
    {
        s->SetGround(sec_pos, o);
        return true;
    }
    return nullptr;
}

void Level::KillFar(const glm::vec3 &pos, float dist)
{
    SectorMap::iterator iter = map.begin();
    for(; iter != map.end(); ) {
        glm::ivec2 off = iter->first;
        glm::vec3 spos = glm::vec3(glm::vec2(off) * glm::vec2(RX, RY), 0);
        if ( glm::distance(pos, spos) >= dist ) {
                map.erase(iter++);
        } else {
                ++iter;
        }
    }
}

void Level::DamageBlock(const glm::vec3 &pos, int count, GameTimer &gt)
{
    std::shared_ptr<Object> &o = GetObjectByPos(pos);
    if(Block *b = o->GetAgent<Block>())
    {
        b->health -= count;
        if(b->health <= 0)
        {
            o->onDestroy(o, this, pos, gt);
            SetBlockAtPos(pos, DB::Create("air"));
            return;
        }
    }
    o->onDamage(o, this, pos, gt);
}

Sector *Level::GetSectorByPos(const glm::vec3 &p, bool request)
{
    int x = p.x < 0 ? (int(p.x) + 1) / RX - 1 : int(p.x) / RX;
    int y = p.y < 0 ? (int(p.y) + 1) / RY - 1 : int(p.y) / RY;

    return GetSector({x,y}, request);
}

Sector *Level::GetSector(const glm::ivec2 &off, bool request)
{
    auto t = map.find(off);
    if(t != map.end())
        return (*t).second.get();

    if(request)
    {
        auto s = RemoteClient::instance().GetSector( off );
        auto sr = s.get();
        if(sr != nullptr)
            map.insert( std::make_pair( off, std::move( s ) ) );
        return sr;
    }
    return nullptr;
}

bool Level::TpCreature( Object &o, const glm::vec3 &pos, bool wait )
{
    Sector *s;
    while( !(s = GetSectorByPos( pos )) && wait )
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    if( !s )
        return false;

    int sec_x = pos.x < 0 ? (int(pos.x) + 1) / RX - 1 : int(pos.x) / RX;
    int sec_y = pos.y < 0 ? (int(pos.y) + 1) / RY - 1 : int(pos.y) / RY;
    auto sec_pos = pos - glm::vec3(sec_x*RX, sec_y*RY, 0);
    if(std::shared_ptr<Object> &targ = s->GetBlock( sec_pos ))
    {
        if( auto w = static_cast<ObjectStatic*>(targ->base)->GetAgent<Walkable>() )
            o.GetAgent<Creature>()->pos = pos;
    }

    return false;
}

bool Level::DeltaCreature(Object &o, const glm::vec3 &delta, bool wait )
{
    auto pos = o.GetAgent<Creature>()->pos;
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
        if(std::shared_ptr<Object> &o = GetObjectByPos(pos))
        {
            o->onLeave(o, this, pos, GameTimer());
        }
        if(std::shared_ptr<Object> &o = GetObjectByPos(new_pos))
        {
            o->onEnter(o, this, new_pos, GameTimer());
        }
    }

    int sec_x = new_pos.x < 0 ? (int(new_pos.x) + 1) / RX - 1 : int(new_pos.x) / RX;
    int sec_y = new_pos.y < 0 ? (int(new_pos.y) + 1) / RY - 1 : int(new_pos.y) / RY;
    auto sec_pos = new_pos - glm::vec3( sec_x*RX, sec_y*RY, 0 );
    std::pair<std::shared_ptr<Object>, std::shared_ptr<Object>> targ = s->GetCell( sec_pos );
    if( targ.first )
    {
        static const ObjectStatic *air = DB::Get("air");

        Walkable *w = targ.first->base->GetAgent<Walkable>();
        if( w && targ.second->base != air )
            o.GetAgent<Creature>()->pos = pos + delta / w->cost;
    }

    return false;
}

void Level::Update(GameTimer& gt)
{
    for(const auto &i : DB::sounds)
    {
        i->onUpdate(std::shared_ptr<Object>(), this, {0,0,0}, gt);
    }

    for(const auto &i : map)
    {
        Sector &cur = *i.second;
        cur.Update(this, gt);

        for(auto c_iter = std::begin(cur.entities); c_iter != std::end(cur.entities);)
        {
            Object &c = **c_iter;
            glm::vec3 pos = c.GetAgent<Creature>()->pos;
            c.onUpdate(*c_iter, this, pos, gt);

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

#ifdef CLIENT
void Level::Draw(SpriteBatch &sb, const glm::vec2 &off, glm::vec3 &hpos) const
{
    for(const auto &i : map)
    {
        auto max = (glm::vec2(i.second->offset) + glm::vec2(1.f, 1.f)) * glm::vec2(RX*sscale, RY*sscale) - off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = glm::vec2(i.second->offset) * glm::vec2(RX*sscale, RY*sscale) - off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->Draw(sb, off, hpos);
    }

    for(const auto &i : map)
    {
        auto max = (glm::vec2(i.second->offset) + glm::vec2(1.f, 1.f)) * glm::vec2(RX*sscale, RY*sscale) - off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = glm::vec2(i.second->offset) * glm::vec2(RX*sscale, RY*sscale) - off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->DrawShadow(sb, off, hpos);
    }

    for(const auto &i : map)
    {
        auto max = (glm::vec2(i.second->offset) + glm::vec2(1.f, 1.f)) * glm::vec2(RX*sscale, RY*sscale) - off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = glm::vec2(i.second->offset) * glm::vec2(RX*sscale, RY*sscale) - off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->DrawBlock(sb, off, hpos);
    }

    for(const auto &i : map)
    {
        auto max = (glm::vec2(i.second->offset) + glm::vec2(1.f, 1.f)) * glm::vec2(RX*sscale, RY*sscale) - off;
        if(max.x < 0 || max.y < 0)
            continue;
        auto min = glm::vec2(i.second->offset) * glm::vec2(RX*sscale, RY*sscale) - off;
        if(min.x > RESX || min.y > RESY)
            continue;
        i.second->DrawEntities(sb, off, hpos);
    }
}
#endif

bool Level::AddCreature( std::shared_ptr<Object> o, bool wait )
{
    if( const Creature *e = o->GetAgent<Creature>() )
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
        LOG(error) << "not an Creature";
}

