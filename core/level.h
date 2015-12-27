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

    void Update(GameTimer &gt);
    void Draw(SpriteBatch &sb, const glm::vec2 &off, glm::vec3 &hpos) const;

    Sector *GetSectorByPos(const glm::vec3 &coord);
    Sector *GetSector(const glm::ivec2 &off);
    Object *GetObjectByPos(const glm::vec3 &p);
    bool SetObjectAtPos(const glm::vec3 &p, std::shared_ptr<Object> o);
    void KillFar(const glm::vec3 &pos, float dist);

    bool AddEntity(std::shared_ptr<Object> o, bool wait = false);
    bool TpEntity(Object &o, const glm::vec3 &pos, bool wait = false);
    bool DeltaEntity(Object &o, const glm::vec3 &delta, bool wait = false);
};

#endif // LEVEL_H
