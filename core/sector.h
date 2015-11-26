#ifndef SECTOR_H
#define SECTOR_H
#include "object.h"
#include "objectstatic.h"
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <list>

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
public:
    Sector(const glm::vec2 &o);

    std::array<const Object*, 6> Neighbours(const glm::ivec3 &pos);
    void Generate();
    void Update();
    void Draw(SpriteBatch &sb, const glm::vec2 &off) const;
    void SetObject(const glm::ivec3 &pos, std::unique_ptr<Object> obj);

private:

    void RebuildMax();

    std::array<std::unique_ptr<Object>, RXYZ> data;
    //std::list<Object*> active;

    glm::ivec2 offset;
    int maxlevel = 0;
};

#endif // SECTOR_H
