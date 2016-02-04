#ifndef EVENTISSUREORDER_H
#define EVENTISSUREORDER_H
#include "event.h"

class EventIssureOrder : public Event
{
public:
    EventIssureOrder();

    void Serialize(rapidjson::Document &doc, rapidjson::Value &v);
    void Deserialize(const rapidjson::Value &val);
};

#endif // EVENTISSUREORDER_H
