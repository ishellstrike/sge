#ifndef REMOTESERVER_H
#define REMOTESERVER_H
#include "logger.h"

#include <boost/noncopyable.hpp>
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include "../offset.h"

class Sector;

class RemoteServer : public boost::noncopyable
{
public:
    RemoteServer();

    static RemoteServer &instance()
    {
        static auto inst = new RemoteServer();
        return *inst;
    }

    std::unordered_map<glm::ivec2, std::shared_ptr<Sector>> ready;
    std::unordered_set<glm::ivec2> requested;

    std::shared_ptr<Sector> GetSector(const glm::ivec2 &v);
    void Process();
};

#endif // REMOTESERVER_H
