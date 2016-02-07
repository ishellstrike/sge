#include "emplacers.h"

Emplacers::Emplacers()
{

}

void Emplacers::Update(const GameTimer &gt)
{
    static const float sync_in_sec = 1;
    size_t min = glm::min(current.size(), upcomming.size());
    for(size_t i = 0; i < min; ++i)
    {
        current[i].pos = glm::mix(current[i].pos, upcomming[i].pos, glm::min(1.f, (gt.current - last_sync)/sync_in_sec));
    }

    if(current.size() < upcomming.size())
    {
        current.insert(current.end(), upcomming.begin() + current.size(), upcomming.end());
    }

    last_update = gt.current;
}

void Emplacers::Sync(const std::vector<PlayerEmplacer> &e, const GameTimer &gt)
{
    upcomming = e;
    last_sync = gt.current;
}

