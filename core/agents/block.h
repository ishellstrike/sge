#ifndef BLOCK_H
#define BLOCK_H
#include "core/agent.h"

class Block : public DynamicAgent
{
public:
    DAGENT(Block)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() const override;

    int health = 1;
};

REGISTER_AGENT(Block)

#endif // BLOCK_H
