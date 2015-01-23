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
    double frameTime = gt.elapsed;

    fpsTime += frameTime;
    fpsStack.push_back(frameTime);

    while(fpsTime > 1)
    {
        fpsTime -= fpsStack.front();
        fpsStack.pop_front();
    }
}

unsigned int FPSCounter::GetCount() const
{
        return fpsStack.size();
}
