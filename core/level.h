#ifndef LEVEL_H
#define LEVEL_H
#include "sector.h"
#include "offset.h"
#include <unordered_map>
#include "core/agents/agents.hpp"

class Level
{
public:
    Level();

    typedef std::unordered_map<glm::ivec2, std::unique_ptr<Sector>> SectorMap;
    SectorMap map;

    Sector *GetSector(const glm::vec2 &off);
    void Update();
    void Draw(SpriteBatch &sb) const;

    glm::ivec2 off;
};

#endif // LEVEL_H
