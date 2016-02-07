/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef CJARG_PERFOMANCE_H
#define CJARG_PERFOMANCE_H
#include "sge_ui/wins.h"
#include "sge_ui/wins.h"
#include "fpscounter.h"
#include <list>
#include "sge_ui/graph.h"
#include "sge_ui/label.h"

class sge_perfomance : public Win
{
public:
    sge_perfomance(WContainer *par);

    void Draw() const;
    void Update(const GameTimer& gt, const MouseState &ms) override;
    void UpdateTimer(const FPSCounter &fps, const GameTimer& gt);
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

#endif // CJARG_PERFOMANCE_H
