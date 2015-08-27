/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef FPSCOUNTER_h__
#define FPSCOUNTER_h__

#include <list>
#include "gametimer.h"

class FPSCounter
{
private:
    std::list<float> fpsStack;

    double fpsTime;

public:
    FPSCounter(void);
    ~FPSCounter(void);

    void Update(GameTimer gt);

    unsigned int GetCount() const;

};


#endif // FPSCOUNTER_h__
