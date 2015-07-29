#ifndef SPACESYSTEM_H
#define SPACESYSTEM_H
#include <vector>
#include <memory>

class SpaceObject;

class SpaceSystem
{
public:
    SpaceSystem();
    std::vector<std::shared_ptr<SpaceObject>> system;
};

#endif // SPACESYSTEM_H
