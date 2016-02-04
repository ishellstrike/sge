#ifndef EVENTBUS_H
#define EVENTBUS_H
#include "event.h"
#include <memory>
#include <boost/noncopyable.hpp>
#include <list>

class Eventbus : boost::noncopyable
{
public:
    Eventbus();

    static Eventbus & Instance()
    {
        static auto eb = std::unique_ptr<Eventbus>(new Eventbus);
        return *eb;
    }

    void PushEvent(std::unique_ptr<Event> &e);
    std::list<std::unique_ptr<Event>> events;
};

#endif // EVENTBUS_H
