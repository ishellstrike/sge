#ifndef SGE_EVENTBUS_LOG_H
#define SGE_EVENTBUS_LOG_H
#include "sge_ui/wins.h"
#include "sge_ui/label.h"
#include <boost/circular_buffer.hpp>

class sge_eventbus_log : public Win
{
public:
    sge_eventbus_log(WContainer *par);

    void Draw() const;
    void Update(const GameTimer& gt, const MouseState &ms) override;

    Label *eb;
    boost::circular_buffer<std::string> cb;

    void* operator new(size_t i)
    {
        return _mm_malloc(i,16);
    }

    void operator delete(void* p)
    {
        _mm_free(p);
    }
};

#endif // SGE_EVENTBUS_LOG_H
