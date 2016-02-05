#ifndef EVENTDAMAGE_H
#define EVENTDAMAGE_H
#include "event.h"

class EventDamage : public Event
{
public:
    EventDamage(float d);
    float damage;

    virtual void Serialize(rapidjson::Document &doc, rapidjson::Value &v);
    virtual void Deserialize(const rapidjson::Value &val);
    std::string to_string() const override;
};

#endif // EVENTDAMAGE_H
