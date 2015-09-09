#ifndef POPULATION_H
#define POPULATION_H
#include <list>
#include "country.h"

class Population
{
public:
    Population();

    std::list<Country> countries;
};

#endif // POPULATION_H
