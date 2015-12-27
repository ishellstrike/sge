#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include "core/agent.h"
#include "core/object.h"

class Order
{
public:
    enum Type {
        Nothing,
        Wander,
        Walk,
        Attack
    };

    Order() {}
    Order(const glm::vec3 &p, Type t) : pos(p), type(t) {}

    glm::vec3 pos;
    Type type = Nothing;
};

class Entity : public DynamicAgent
{
public:
    DAGENT(Entity)

    std::vector<std::unique_ptr<Object>> items;

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;
    virtual void onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer &gt);

    glm::vec3 pos;
    Order current_order;
};

REGISTER_AGENT(Entity)

#endif // ENTITY_H
