/*******************************************************************************
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

void FPSCounter::Update(GameTimer gt)
{
    fpsTime = 1.0 / gt.elapsed;
}

unsigned int FPSCounter::GetCount() const
{
        return fpsTime;
}
