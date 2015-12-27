#include "block.h"

void Block::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(health));
}

std::shared_ptr<Agent> Block::Instantiate() const
{
    auto b = std::make_shared<Block>();
    b->health = health;
    return b;
}
