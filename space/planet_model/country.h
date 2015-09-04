#ifndef COUNTRY_H
#define COUNTRY_H
#include <vector>
#include "city.h"

class Country
{
public:
    Country();

    std::vector<City> cities;
};

#endif // COUNTRY_H
