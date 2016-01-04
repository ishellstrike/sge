#ifndef CHEST_H
#define CHEST_H
#include <vector>
#include "core/agent.h"
#include "core/object.h"

class Chest : public DynamicAgent
{
public:
    DAGENT(Chest)

    std::vector<std::shared_ptr<Object>> items;

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;
    void onInteract(std::shared_ptr<Object> &par, Level *, const glm::vec3 &, const GameTimer&) override;

    void Combine();
};

REGISTER_AGENT(Chest)

#endif // CHEST_H
