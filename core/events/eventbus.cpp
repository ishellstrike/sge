#include "eventbus.h"

Eventbus::Eventbus()
{

}

void Eventbus::PushEvent(std::unique_ptr<Event> &e)
{
    events.push_back(std::move(e));
}

