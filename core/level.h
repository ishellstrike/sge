#ifndef LEVEL_H
#define LEVEL_H
#include "sector.h"
#include "offset.h"
#include <unordered_map>

class Level
{
public:
    Level();

    std::unordered_map<glm::ivec2, std::unique_ptr<Sector>> map;

    Sector *GetSector(const glm::vec2 &off);
    void Update();
    void Draw(SpriteBatch &sb) const;

    glm::vec2 off;
};

#endif // LEVEL_H
