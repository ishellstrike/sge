#ifndef LEVEL_H
#define LEVEL_H
#include "sector.h"
#include "offset.h"
#include <unordered_map>
#include "core/agents/agents.hpp"

class Level
{
    friend class sge_level_debug_info;
public:
    Level();

    typedef std::unordered_map<glm::ivec2, std::unique_ptr<Sector>> SectorMap;
    SectorMap map;

    Sector *GetSector(const glm::vec2 &off);
    void Update();
    void Draw(SpriteBatch &sb, const glm::vec2 &off) const;
    void AddEntity(std::unique_ptr<Object> o);

    Sector *GetSectorByPos(const glm::vec3 &coord);
};

#endif // LEVEL_H
