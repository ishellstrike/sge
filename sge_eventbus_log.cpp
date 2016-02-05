#include "sge_eventbus_log.h"

#include "sge_settings_main.h"
#include "prefecences.h"
#include <core/events/eventbus.h>
#include <glm/gtx/string_cast.hpp>
#include <core/serialize.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

sge_eventbus_log::sge_eventbus_log(WContainer *par) :
    Win(par), cb(30)
{
    eb = new Label(this);
}

void sge_eventbus_log::Draw() const
{
    Win::Draw();
}

void sge_eventbus_log::Update(const GameTimer &gt, const MouseState &ms)
{
    for(const auto &e : Eventbus::Instance().events)
    {
        cb.push_back(e->to_string());
    }
    std::stringstream ss;
    for(const auto &s : cb)
    {
        ss << s << "\n";
    }

    eb->text(ss.str());
    size = eb->size + glm::vec2(5,22);

    Win::Update(gt, ms);
}

