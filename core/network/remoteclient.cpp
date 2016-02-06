#include "remoteclient.h"
#include "../levelgen.h"
#include <thread>
#include "packetrequestsector.h"

RemoteClient::RemoteClient()
{
    hive = std::make_shared<Hive>();
    conn = std::make_shared<MyConnection>(hive);
    conn->Connect("127.0.0.1", 8080);
}

std::unique_ptr<Sector> RemoteClient::GetSector(const glm::ivec2 &v)
{
    auto f = ready.find( v );
    if( f != ready.end() )
    {
        std::pair<glm::ivec2, std::unique_ptr<Sector>> sec = std::move( *f );
        ready.erase( f );
        return std::move( sec.second );
    }

    auto r = requested.find( v );
    if( r != requested.end() )
    {
        return nullptr;
    }

    requested.insert( v );
    return nullptr;
}

void RemoteClient::Process()
{
    hive->Poll();
    if( requested.size() )
    {
        auto r = requested.begin();
        auto a = std::unique_ptr<Sector>( new Sector( *r ) );
        Generate( *a );

        std::unique_ptr<Packet> prs( new PacketRequestSector(*r) );
        conn->Send( Packet::Serealize(*prs) );
        ready.insert( std::make_pair( *r, std::move( a ) ) );

        requested.erase( r );
    }
}



void MyConnection::OnAccept(const std::string &host, uint16_t port)
{
    LOG(info) << "[" << __FUNCTION__ << "] " << host << ":" << port;

    // Start the next receive
    Recv();
}

void MyConnection::OnConnect(const std::string &host, uint16_t port)
{
    LOG(info) << "[" << __FUNCTION__ << "] " << host << ":" << port;

    // Start the next receive
    Recv();
}

void MyConnection::OnSend(const std::vector<uint8_t> &buffer)
{
    LOG(info) << std::string(buffer.begin(), buffer.end());
}

void MyConnection::OnRecv(std::vector<uint8_t> &buffer)
{
    LOG(info) << std::string(buffer.begin(), buffer.end());

    // Start the next receive
    Recv();
}

void MyConnection::OnTimer(const boost::posix_time::time_duration &delta)
{
    //std::cout << "[" << __FUNCTION__ << "] " << delta << std::endl;
}

void MyConnection::OnError(const boost::system::error_code &error)
{
    LOG(info) << "[" << __FUNCTION__ << "] " << error;
}

MyConnection::MyConnection(std::shared_ptr<Hive> hive)
    : Connection( hive )
{
}

MyConnection::~MyConnection()
{
}
