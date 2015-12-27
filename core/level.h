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
    Sector *GetSector(const glm::ivec2 &off, bool request = true);
    Object *GetObjectByPos(const glm::vec3 &p);
    std::list<Sector *> GetSectorsInRange(const glm::vec3 &coord, float r);
    std::list<std::weak_ptr<Object>> GetObjectsInRange(const glm::vec3 &coord, float r);
    bool SetObjectAtPos(const glm::vec3 &p, std::shared_ptr<Object> o);
    void KillFar(const glm::vec3 &pos, float dist);

    void DamageBlock(const glm::vec3 &pos, int count, GameTimer &gt);

    bool AddCreature(std::shared_ptr<Object> o, bool wait = false);
    bool TpCreature(Object &o, const glm::vec3 &pos, bool wait = false);
    bool DeltaCreature(Object &o, const glm::vec3 &delta, bool wait = false);
};

#endif // LEVEL_H
