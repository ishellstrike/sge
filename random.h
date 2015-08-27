/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef RANDOM_H
#define RANDOM_H
#include <random>

class random
{
    static std::mt19937 generator;
    static std::uniform_real_distribution<float> fdist;
    static std::uniform_real_distribution<double> ddist;
    static std::normal_distribution<float> nfdist;
    static std::normal_distribution<double> nddist;
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

    template<typename T>
    static T norm()
    {
        static_assert(0, "Not implemented");
        return T(0);
    }

    template<>
    static float norm()
    {
        return nfdist(generator);
    }

    template<>
    static double norm()
    {
        return nddist(generator);
    }
};

#endif // RANDOM_H
