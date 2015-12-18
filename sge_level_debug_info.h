#ifndef SGE_LEVEL_DEBUG_INFO_H
#define SGE_LEVEL_DEBUG_INFO_H
#include "sge_ui/wins.h"
#include "fpscounter.h"
#include "gametimer.h"
#include <list>
#include "sge_ui/graph.h"
#include "sge_ui/label.h"
#include "core/level.h"

class sge_level_debug_info : public Win
{
public:
    sge_level_debug_info(WContainer *par);

    void Draw() const;
    void Update(const GameTimer &gt, const MouseState &ms) override;
    void UpdateLevelInfo(const Level &fps);
    Graph *gfps, *ggt, *gfps_fast, *ggt_fast;
    Label *fps1, *fps2, *time1, *time2;
    float fastsec = 0;
    float compose_fps = 0, compose_gt = 0;
    int compose = 0;

    void* operator new(size_t i)
    {
        return _mm_malloc(i,16);
    }

    void operator delete(void* p)
    {
        _mm_free(p);
    }
};

#endif // SGE_LEVEL_DEBUG_INFO_H
