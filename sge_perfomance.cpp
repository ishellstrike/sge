/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "sge_perfomance.h"

sge_perfomance::sge_perfomance(WContainer *par) :
    Win(par)
{
    text = "sge_perfomance";
    size = {200,220};
    pos = {0,0};
    resizable = true;

    fps1 = new Label(this);
    fps2 = new Label(this);
    time1 = new Label(this);
    time2 = new Label(this);

    fps1->text("FPS");
    fps1->align(SpriteBatch::ALIGN_CENTER);
    fps2->text("FPS");
    fps2->align(SpriteBatch::ALIGN_CENTER);
    time1->text("FRAME TIME");
    time1->align(SpriteBatch::ALIGN_CENTER);
    time2->text("FRAME TIME");
    time2->align(SpriteBatch::ALIGN_CENTER);

    ggt = new Graph(this);
    gfps = new Graph(this);
    ggt_fast = new Graph(this);
    gfps_fast = new Graph(this);

    onResize.connect([=]{
        glm::vec2 size = this->size - glm::vec2(0,20);
        ggt->size = gfps->size = ggt_fast->size = gfps_fast->size = size/2.f;
        gfps->pos = {size.x/2.f, 0};
        ggt_fast->pos = {0, size.y/2.f};
        gfps_fast->pos = {size.x/2.f, size.y/2.f};

        fps1->size = time1->size = time2->size = fps2->size = gfps->size;
        fps1->pos = gfps->pos;
        fps2->pos = gfps_fast->pos;
        time1->pos = ggt->pos;
        time2->pos = ggt_fast->pos;
    });

    onResize();
}

void sge_perfomance::Draw() const
{
    Win::Draw();
}

void sge_perfomance::Update(const GameTimer &gt, const MouseState &ms)
{
    Win::Update(gt, ms);
}

void sge_perfomance::UpdateTimer(const FPSCounter &fps, const GameTimer &gt)
{
    fastsec += gt.elapsed;


    if(fastsec > 0.01)
    {
        ggt_fast->AddValue(gt.elapsed);
        gfps_fast->AddValue(fps.GetCount());
        fastsec = 0;
        compose_fps += fps.GetCount();
        compose_gt += gt.elapsed;
        compose ++;
        if(compose >= 9)
        {
            ggt->AddValue(compose_gt/10.f);
            gfps->AddValue(compose_fps/10.f);
            compose_fps = 0;
            compose_gt = 0;
            compose = 0;
        }
    }
}
