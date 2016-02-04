#include "eventissureorder.h"
#include <core/serialize.h>

EventIssureOrder::EventIssureOrder()
{

}

void EventIssureOrder::Serialize(rapidjson::Document &doc, rapidjson::Value &v)
{
    SERIALIZE(NvpHelper::make_nvp("class", "EventIssureOrder"));
    Event::Serialize(doc, v);
}

void EventIssureOrder::Deserialize(const rapidjson::Value &val)
{
    Event::Deserialize(val);
}
