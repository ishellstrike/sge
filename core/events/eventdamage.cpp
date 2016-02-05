#include "eventdamage.h"
#include <core/serialize.h>
#include <sstream>
#include <glm/gtx/string_cast.hpp>

EventDamage::EventDamage(float d) : damage(d)
{

}

void EventDamage::Serialize(rapidjson::Document &doc, rapidjson::Value &v)
{

}

void EventDamage::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(damage));
    Event::Deserialize(val);
}

std::string EventDamage::to_string() const
{
    std::stringstream ss;
    ss << "EventDamage " << Event::to_string() << " damage: " << damage;
    return ss.str();
}
