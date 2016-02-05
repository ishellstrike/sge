#ifndef EVENTISSUREORDER_H
#define EVENTISSUREORDER_H
#include "event.h"
#include "core/agents/entity.h"

class EventIssureOrder : public Event
{
public:
    EventIssureOrder();

    void Serialize(rapidjson::Document &doc, rapidjson::Value &v);
    void Deserialize(const rapidjson::Value &val);
    std::string to_string() const override;
};

#endif // EVENTISSUREORDER_H
