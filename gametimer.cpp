/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

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

std::ostream& operator <<(std::ostream& os, const GameTimer& dt)
{
    os << "{cur: " << dt.current << ", delta: " << dt.elapsed << "}";
    return os;
}


