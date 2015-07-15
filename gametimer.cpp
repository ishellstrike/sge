#include "gametimer.h"

void GameTimer::Update(float wglTime)
{
    last = current;
    current = wglTime;
    elapsed = current - last;
}

GameTimer::GameTimer()
{
    last = 0;
    current = 0;
    elapsed = 0;
}

GameTimer::~GameTimer()
{

}


