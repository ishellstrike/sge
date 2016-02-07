#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H
#include <boost/noncopyable.hpp>
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include "../offset.h"
#include "core/emplacers.h"

#include "network.h"

class MyConnection : public Connection
{
private:
    void OnAccept( const std::string & host, uint16_t port );

    void OnConnect( const std::string & host, uint16_t port );

    void OnSend( const std::vector< uint8_t > & buffer );

    void OnRecv( std::vector< uint8_t > & buffer );

    void OnTimer( const boost::posix_time::time_duration & delta );

    void OnError( const boost::system::error_code & error );

public:
    MyConnection( std::shared_ptr< Hive > hive );
    std::vector<PlayerEmplacer> temp_emp;

    ~MyConnection();
};

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

    std::unordered_map<glm::ivec2, std::shared_ptr<Sector>> ready;
    std::unordered_set<glm::ivec2> requested;

    std::shared_ptr<Sector> GetSector(const glm::ivec2 &v);

	std::vector<PlayerEmplacer> GetPlayerEmplacers(glm::vec3 pos, float phi, size_t id);
    void Process();
    std::shared_ptr<MyConnection> conn;
    std::shared_ptr<Hive> hive;
};

#endif // REMOTECLIENT_H
