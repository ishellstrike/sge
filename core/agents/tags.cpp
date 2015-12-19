#include "tags.h"



void Tags::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(tags));
}

std::shared_ptr<Agent> Tags::Instantiate()
{
    return std::make_shared<Tags>();
}

bool Tags::IsStatic()
{
    return true;
}
