#include "eventdamage.h"
#include <core/serialize.h>


EventDamage::EventDamage(float d) : damage(d)
{

}

void EventDamage::Serialize(rapidjson::Document &doc, rapidjson::Value &v)
{
    SERIALIZE(NVP(damage));
    SERIALIZE(NvpHelper::make_nvp("class", "EventDamage"));
    Event::Serialize(doc, v);
}

void EventDamage::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(damage));
    Event::Deserialize(val);
}
