#ifndef RANDOM_H
#define RANDOM_H
#include <random>

class random
{
    static std::mt19937 generator;
    static std::uniform_real_distribution<float> fdist;
    static std::uniform_real_distribution<double> ddist;
public:
    random() = delete;

    template<typename T>
    static T next()
    {
        static_assert(0, "Not implemented");
        return T(0);
    }

    template<>
    static float next()
    {
        return fdist(generator);
    }

    template<>
    static double next()
    {
        return ddist(generator);
    }
};

#endif // RANDOM_H
