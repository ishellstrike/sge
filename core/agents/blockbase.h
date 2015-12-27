#ifndef BLOCKBASE_H
#define BLOCKBASE_H
#include "core/agent.h"
#include "material.h"

class BlockBase : public StaticAgent
{
public:
    SAGENT(BlockBase)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    void onLoad(Object *par, Level *, const glm::vec3 &pos, const GameTimer &gt) override;
    void onDamage(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt) override;
    void onDestroy(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt) override;

    int health = 1;
    std::string material;
    Material *mat;
};

REGISTER_AGENT(BlockBase)

#endif // BLOCKBASE_H
