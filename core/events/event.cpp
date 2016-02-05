#include "event.h"
#include "core/serialize.h"
#include <functional>
#include <sstream>
#include <glm/gtx/string_cast.hpp>

Event::Event(const Event::TargetPoint &p) : type(Event::TARGET_POINT), data(p)
{

}

Event::Event(const Event::TargetRange &p) : type(Event::TARGET_RANGE), data(p)
{

}

Event::Event(const Uid &p) : type(Event::TARGET_OBJECT), data(p)
{

}

Event::Event() : type(Event::NO_TARGET)
{

}

Event::~Event()
{

}

void Event::Set(const Event::TargetPoint &p)
{
    type = Event::TARGET_POINT;
    data = p;
}

void Event::Set(const Event::TargetRange &p)
{
    type = Event::TARGET_RANGE;
    data = p;
}

void Event::Set(const Uid &p)
{
    type = Event::TARGET_OBJECT;
    data = p;
}

void Event::Set()
{
    type = Event::NO_TARGET;
    data.clear();
}

void Event::Serialize(rapidjson::Document &doc, rapidjson::Value &v)
{

}

void Event::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(type));
    switch(type)
    {
        case TARGET_POINT:
          //DESERIALIZE(NVP(boost::any_cast<TargetPoint>(data)));
        break;
        case TARGET_RANGE:
          //DESERIALIZE(DeserializeHelper::make_nvp("data", boost::any_cast<TargetRange>(data)));
        break;
        case TARGET_OBJECT:
          //DESERIALIZE(NVP(boost::any_cast<std::string>(data)));
        break;
    }
}

std::string Event::to_string() const
{
    std::stringstream ss;
    switch (type)
    {
    case NO_TARGET:
        ss << "no target";
        break;
    case TARGET_POINT:
        ss << "target point: " << glm::to_string(boost::any_cast<TargetPoint>(data).pos);
        break;
    case TARGET_RANGE:
        ss << "target range: " << glm::to_string(boost::any_cast<TargetRange>(data).pos)
           << " r: " << boost::any_cast<TargetRange>(data).range;
        break;
    case TARGET_OBJECT:
        ss << "target: " << boost::any_cast<Uid>(data);
        break;
    }

    return ss.str();
}

Event::TargetPoint::TargetPoint(const glm::vec3 &p) : pos(p)
{

}

Event::TargetPoint::TargetPoint()
{

}

void Event::TargetPoint::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(pos));
}

void Event::TargetPoint::Serialize(rapidjson::Document &doc, rapidjson::Value &v)
{

}


void Event::TargetRange::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(pos), NVP(range));
}

void Event::TargetRange::Serialize(rapidjson::Document &doc, rapidjson::Value &v)
{

}
