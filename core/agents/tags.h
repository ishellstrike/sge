#ifndef TAGS_H
#define TAGS_H
#include "core/agent.h"

class Tags : public Agent
{
public:
    AGENT(Tags)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::shared_ptr<Agent> Instantiate() override;

    bool IsStatic() override;

    std::vector<std::string> tags;
};

REGISTER_AGENT(Tags)

#endif // TAGS_H
