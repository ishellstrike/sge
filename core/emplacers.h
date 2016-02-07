#ifndef EMPLACERS_H
#define EMPLACERS_H
#include "playeremplacer.h"
#include <vector>
#include "gametimer.h"

class Emplacers
{
public:
    Emplacers();
    void Update(const GameTimer &gt);
    void Sync(const std::vector<PlayerEmplacer> &e, const GameTimer &gt);

    float last_sync = 0, last_update = 0;
    std::vector<PlayerEmplacer> current;
    std::vector<PlayerEmplacer> upcomming;
};

#endif // EMPLACERS_H
