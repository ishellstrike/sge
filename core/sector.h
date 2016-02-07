#ifndef SECTOR_H
#define SECTOR_H
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>
#include <array>
#include <memory>
#include <list>

#include "spritebatch.h"
#include "db.h"

#define RX 16
#define RY 16
#define RZ 4
#define RXYZ RX*RY*RZ
#define RYZ RY*RZ
#define ONEDIM(x,y,z) x*RYZ + y*RZ + z

class Sector
{
    friend class Level;
    friend class RemoteClient;
    friend void Generate(Sector &s);

public:
    Sector(const glm::ivec2 &o);
    Sector();

    std::array<std::shared_ptr<Object>, 6> Neighbours(const glm::ivec3 &pos);
    void Update(Level *l, GameTimer &gt);

#ifdef CLIENT
    void Draw(SpriteBatch &sb, const glm::ivec2 &off, const glm::vec3 &hpos) const;
    void DrawEntities(SpriteBatch &sb, const glm::ivec2 &off, const glm::vec3 &hpos) const;
    void DrawShadow(SpriteBatch &sb, const glm::ivec2 &off, const glm::vec3 &hpos) const;
    void DrawBlock(SpriteBatch &sb, const glm::ivec2 &off, const glm::vec3 &hpos) const;
#endif

    void SetBlock(const glm::ivec3 &pos, std::shared_ptr<Object> obj);

    void PlaceScheme(const Scheme &s, const glm::ivec3 &pos);

    glm::ivec2 offset;
    std::shared_ptr<Object> GetBlock(const glm::ivec3 &pos);
    std::pair<std::shared_ptr<Object>, std::shared_ptr<Object> > GetCell(const glm::ivec3 &pos);
    std::shared_ptr<Object> GetGround(const glm::ivec3 &pos);
    void SetGround(const glm::ivec3 &pos, std::shared_ptr<Object> obj);
private:

    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive &ar, const unsigned int) const
    {
        ar << offset;
        for(int i = 0; i < RXYZ; ++i)
        {
            ar << block[i]->base->id;
            ar << ground[i]->base->id;
        }
    }

    template<class Archive>
    void load(Archive &ar, const unsigned int)
    {
        ar >> offset;
        for(int i = 0; i < RXYZ; ++i)
        {
            Id b, d;
            ar >> b;
            ar >> d;

            block[i] = DB::Create(b);
            ground[i] = DB::Create(d);
        }
    }


    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void RebuildMax();

    std::array<std::shared_ptr<Object>, RXYZ> block, ground;
    std::list<std::shared_ptr<Object>> entities;
    //std::list<Object*> active;
    int maxlevel = 0;
};

#endif // SECTOR_H
