/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef GAMETIMER_H
#define GAMETIMER_H
#include <ostream>

class GameTimer
{
public:
    GameTimer();
    ~GameTimer();
    float last, current, elapsed; /*< in seconds */
    void Update(float wglTime);

    friend std::ostream& operator <<(std::ostream& os, const GameTimer& dt);
};

#endif // GAMETIMER_H
