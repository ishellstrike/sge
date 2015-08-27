/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef GAMETIMER_H
#define GAMETIMER_H


class GameTimer
{
public:
    GameTimer();
    ~GameTimer();
    float last, current, elapsed; /*< in seconds */
    void Update(float wglTime);
};

#endif // GAMETIMER_H
