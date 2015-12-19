#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H
#include <boost/noncopyable.hpp>
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include "offset.h"

class Sector;

class RemoteClient : public boost::noncopyable
{
public:
    RemoteClient();

    static RemoteClient &instance()
    {
        static auto inst = new RemoteClient();
        return *inst;
    }

    std::unordered_map<glm::ivec2, std::unique_ptr<Sector>> ready;
    std::unordered_set<glm::ivec2> requested;

    std::unique_ptr<Sector> GetSector(const glm::ivec2 &v);
    void Process();
};

#endif // REMOTECLIENT_H
