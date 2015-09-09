#ifndef COUNTRY_H
#define COUNTRY_H
#include <list>
#include "city.h"

class Country
{
public:
    Country();

    std::list<City> cities;
};

#endif // COUNTRY_H
