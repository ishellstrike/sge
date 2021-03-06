/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "random.h"
#include <time.h>

std::mt19937 random::generator(static_cast<unsigned int>(time(0)));
std::uniform_real_distribution<float> random::fdist(0,1);
std::uniform_real_distribution<double> random::ddist(0,1);
std::normal_distribution<float> random::nfdist(0,1);
std::normal_distribution<double> random::nddist(0,1);

