#include "remoteclient.h"
#include "levelgen.h"
#include <thread>

RemoteClient::RemoteClient()
{
    hive = std::make_shared<Hive>();
    conn = std::make_shared<MyConnection>(hive);
    conn->Connect("127.0.0.1", 7777);
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
    if( requested.size() )
    {
        auto r = requested.begin();
        auto a = std::unique_ptr<Sector>( new Sector( *r ) );
        Generate( *a );
        std::string str = "12334412341214";
        std::vector< uint8_t > request;
        std::copy( str.begin(), str.end(), std::back_inserter( request ) );
        conn->Send(request);
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

    std::string str = "GET / HTTP/1.0\r\n\r\n";

    std::vector< uint8_t > request;
    std::copy( str.begin(), str.end(), std::back_inserter( request ) );
    Send( request );
}

void MyConnection::OnSend(const std::vector<uint8_t> &buffer)
{
    std::stringstream ss;
    ss << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
    for( size_t x = 0; x < buffer.size(); ++x )
    {
        ss << std::hex << std::setfill( '0' ) <<
              std::setw( 2 ) << (int)buffer[ x ] << " ";
        if( ( x + 1 ) % 16 == 0 )
        {
            ss << std::endl;
        }
    }
    LOG(info) << ss.str();
}

void MyConnection::OnRecv(std::vector<uint8_t> &buffer)
{
    std::stringstream ss;
    ss << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
    for( size_t x = 0; x < buffer.size(); ++x )
    {
        ss << std::hex << std::setfill( '0' ) <<
              std::setw( 2 ) << (int)buffer[ x ] << " ";
        if( ( x + 1 ) % 16 == 0 )
        {
            ss << std::endl;
        }
    }
    LOG(info) << ss.str();

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
