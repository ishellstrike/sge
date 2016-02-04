#ifndef EVENT_H
#define EVENT_H
#include <glm/glm.hpp>
#include "../core_const.h"
#include <boost/any.hpp>
#include "rapidjson/document.h"

class Event
{
public:

    struct TargetPoint
    {
        TargetPoint(const glm::vec3 &p);
        TargetPoint();
        glm::vec3 pos;
        void Deserialize(const rapidjson::Value &doc);
        void Serialize(rapidjson::Document &doc, rapidjson::Value &v);
    };

    struct TargetRange
    {
        glm::vec3 pos;
        float range;
        void Deserialize(const rapidjson::Value &doc);
        void Serialize(rapidjson::Document &doc, rapidjson::Value &v);
    };

    enum EventType
    {
        NO_TARGET,
        TARGET_POINT,
        TARGET_RANGE,
        TARGET_OBJECT
    };

    Event(const TargetPoint &p);
    Event(const TargetRange &p);
    Event(const Uid &p);
    Event();
    virtual ~Event();

    void Set(const TargetPoint &p);
    void Set(const TargetRange &p);
    void Set(const Uid &p);
    void Set();

    virtual void Serialize(rapidjson::Document &doc, rapidjson::Value &v);
    virtual void Deserialize(const rapidjson::Value &val);

    int type;
    boost::any data;
};

#endif // EVENT_H
