/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef RANDOM_H__
#define RANDOM_H__
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

    template<typename T = float>
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

    template<typename T = float>
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

#endif // RANDOM_H__
