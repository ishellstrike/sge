#ifndef SECTOR_H
#define SECTOR_H
#include "object.h"
#include "objectstatic.h"
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <list>
#include "scheme.h"

#include <boost/archive/basic_archive.hpp>

#include "spritebatch.h"

#define RX 16
#define RY 16
#define RZ 16
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

    std::array<const Object*, 6> Neighbours(const glm::ivec3 &pos);
    void Update();
    void Draw(SpriteBatch &sb, const glm::ivec2 &off) const;
    void SetObject(const glm::ivec3 &pos, std::shared_ptr<Object> obj);

    void PlaceScheme(const Scheme &s, const glm::ivec3 &pos);

    glm::ivec2 offset;
private:

    void RebuildMax();

    std::array<std::shared_ptr<Object>, RXYZ> data;
    std::list<std::shared_ptr<Object>> entities;
    //std::list<Object*> active;
    int maxlevel = 0;
};

#endif // SECTOR_H
