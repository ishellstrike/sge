#ifndef TAGS_H
#define TAGS_H
#include "core/agent.h"

class Tags : public StaticAgent
{
public:
    SAGENT(Tags)

    // Agent interface
public:
    void Deserialize(rapidjson::Value &val) override;
    std::vector<std::string> tags;
};

REGISTER_AGENT(Tags)

#endif // TAGS_H
