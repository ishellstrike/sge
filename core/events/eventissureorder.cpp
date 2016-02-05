#include "eventissureorder.h"
#include <core/serialize.h>
#include <sstream>
#include <glm/gtx/string_cast.hpp>

EventIssureOrder::EventIssureOrder()
{

}

void EventIssureOrder::Serialize(rapidjson::Document &doc, rapidjson::Value &v)
{

}

void EventIssureOrder::Deserialize(const rapidjson::Value &val)
{
    Event::Deserialize(val);
}

std::string EventIssureOrder::to_string() const
{
    std::stringstream ss;
    ss << "EventIssureOrder " << Event::to_string();
    return ss.str();
}
