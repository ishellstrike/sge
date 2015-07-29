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
