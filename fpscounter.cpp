﻿/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "fpscounter.h"
#include <GLFW/glfw3.h>
#include "gametimer.h"

FPSCounter::FPSCounter(void)
{
    fpsTime = 0;
}


FPSCounter::~FPSCounter(void)
{
}

void FPSCounter::Update(const GameTimer& gt)
{
    float frameTime = gt.elapsed;

    fpsTime += frameTime;
    fpsStack.push(frameTime);

    while(fpsTime > 1)
    {
        fpsTime -= fpsStack.front();
        fpsStack.pop();
    }
}

unsigned int FPSCounter::GetCount() const
{
        return fpsStack.size();
}
