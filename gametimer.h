#ifndef GAMETIMER_H
#define GAMETIMER_H


class GameTimer
{
public:
    GameTimer();
    ~GameTimer();
    double last, current, elapsed;
    void Update(double wglTime);
};

#endif // GAMETIMER_H
