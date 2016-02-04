#include "event.h"
#include "core/serialize.h"
#include <functional>

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
    SERIALIZE(NVP(type));
    TargetPoint t1;
    TargetRange t2;
    std::string t3;
    switch(type)
    {
        case TARGET_POINT:
        {
            const TargetPoint &t1 = boost::any_cast<TargetPoint>(this->data);
            SERIALIZE(NvpHelper::make_nvp("data", t1));
        }
        break;
        case TARGET_RANGE:
          t2 = boost::any_cast<TargetRange>(this->data);
          SERIALIZE(NvpHelper::make_nvp("data", t2));
        break;
        case TARGET_OBJECT:
          t3 = boost::any_cast<std::string>(this->data);
          SERIALIZE(NvpHelper::make_nvp("data", t3));
        break;
    }
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
    SERIALIZE(NVP(pos));
}


void Event::TargetRange::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(pos), NVP(range));
}

void Event::TargetRange::Serialize(rapidjson::Document &doc, rapidjson::Value &v)
{
    SERIALIZE(NVP(pos));
    SERIALIZE(NVP(range));
}
