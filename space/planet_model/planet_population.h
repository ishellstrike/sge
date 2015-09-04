#ifndef POPULATION_H
#define POPULATION_H
#include <vector>
#include "country.h"

class Population
{
public:
    Population();

    std::vector<Country> countries;
};

#endif // POPULATION_H
