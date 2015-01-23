#ifndef FPSCOUNTER_h__
#define FPSCOUNTER_h__

#include <list>
#include "gametimer.h"

class FPSCounter
{
private:
    std::list<double> fpsStack;

    double fpsTime;

public:
    FPSCounter(void);
    ~FPSCounter(void);

    void Update(GameTimer gt);

    unsigned int GetCount() const;

};


#endif // FPSCOUNTER_h__
