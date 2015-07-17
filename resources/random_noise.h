#ifndef RANDOM_NOISE_H
#define RANDOM_NOISE_H
#include "pixmap.h"

inline Pixmap RandomNoise(glm::vec2 __size)
{
    Pixmap a(__size);
    for(size_t i = 0; i < __size.x; ++i)
    {
        for(size_t j = 0; j < __size.y; ++j)
        {
            a.data[(j*a.height + i)*4+0] = rand() % std::numeric_limits<unsigned char>::max();
            a.data[(j*a.height + i)*4+1] = rand() % std::numeric_limits<unsigned char>::max();
            a.data[(j*a.height + i)*4+2] = rand() % std::numeric_limits<unsigned char>::max();
            a.data[(j*a.height + i)*4+3] = rand() % std::numeric_limits<unsigned char>::max();
        }
    }

    return a;
}
#endif // RANDOM_NOISE_H

