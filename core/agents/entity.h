#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include "core/agent.h"
#include "core/object.h"

class Entity : public DynamicAgent
{
public:
    DAGENT(Entity)

    std::vector<std::unique_ptr<Object>> items;

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;

    glm::vec3 pos;
};

REGISTER_AGENT(Entity)

#endif // ENTITY_H
