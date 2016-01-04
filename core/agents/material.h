#ifndef MATERIAL_H
#define MATERIAL_H
#include "core/agent.h"
#include "sound.h"

class Material : public StaticAgent
{
public:
    SAGENT(Material)

    // Agent interface
public:

    void Deserialize(const rapidjson::Value &val) override;
    void onLoad(std::shared_ptr<Object> &par, Level *, const glm::vec3 &pos, const GameTimer& gt) override;
    void onDamage(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt) override;
    void onDestroy(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt) override;

    std::string breaksound, hitsound;
    const Sound *hitting, *breaking;
};

REGISTER_AGENT(Material)

#endif // MATERIAL_H
