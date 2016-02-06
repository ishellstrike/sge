#include "remoteclient.h"
#include "../levelgen.h"
#include <thread>
#include "packetholder.h"

RemoteClient::RemoteClient()
{
    hive = std::make_shared<Hive>();
    conn = std::make_shared<MyConnection>(hive);
    conn->Connect("127.0.0.1", 8080);
}

std::shared_ptr<Sector> RemoteClient::GetSector(const glm::ivec2 &v)
{
    auto f = ready.find( v );
    if( f != ready.end() )
    {
        std::pair<glm::ivec2, std::shared_ptr<Sector>> sec = std::move( *f );
        return sec.second;
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

        PacketHolder ph;
        ph.Init<PacketRequestSector>( *r );
        conn->Send( ph.Serialize() );

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
}

void MyConnection::OnRecv(std::vector<uint8_t> &buffer)
{
    PacketHolder ph;
    static std::vector<uint8_t> ubf;
    try
    {
        ubf.insert(ubf.end(), buffer.begin(), buffer.end());
        ph.Deserialize(ubf);
        if(ph.packet && ph.packet->id == Packet::TidFor<PacketResponseSector>())
        {
            std::shared_ptr<PacketResponseSector> prs = std::static_pointer_cast<PacketResponseSector>(ph.packet);
            RemoteClient::instance().ready[prs->s->offset] = prs->s;
        }
        ubf.clear();
    }
    catch(...)
    {
    }

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
