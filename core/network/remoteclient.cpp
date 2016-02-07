#include "remoteclient.h"
#include "../levelgen.h"
#include <thread>
#include "prefecences.h"

RemoteClient::RemoteClient()
{
    hive = std::make_shared<Hive>();
    conn = std::make_shared<MyConnection>(hive);
	conn->Connect(Prefecences::Instance()->server_adress, Prefecences::Instance()->server_port);
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

std::vector<PlayerEmplacer> RemoteClient::GetPlayerEmplacers(glm::vec3 pos, float phi, size_t id)
{
    PacketHolder ph;
    ph.Init<PacketRequestPlayers>( pos, phi, id);
    conn->Send( ph.Serialize() );

    return conn->temp_emp;
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

	std::string str = "HELLO";

	std::vector< uint8_t > request;
	std::copy(str.begin(), str.end(), std::back_inserter(request));
	Send(request);
}

void MyConnection::OnSend(const std::vector<uint8_t> &buffer)
{
}

size_t getLength(const std::vector<uint8_t> &buffer)
{
	size_t l = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0] + 6;
	if (buffer[4] != 0x0A || buffer[5] != 0x0D)
		l = 0;

	return l;
}

void MyConnection::OnRecv(std::vector<uint8_t> &buffer)
{
    PacketHolder ph;
    static std::vector<uint8_t> ubf;
	static std::vector<uint8_t> tail;
	static size_t length_header = 0;
    try
    {
		if (ubf.empty())
		{
			ubf.insert(ubf.end(), buffer.begin(), buffer.end());
			length_header = getLength(ubf);
			if (length_header == 0)
				LOG(info) << "hz chto, stream skipped";
		}
		else
		{
			ubf.insert(ubf.end(), buffer.begin(), buffer.end());
		}

		if (ubf.size() >= length_header) //enough accepted
		{
			if (ubf.size() > length_header) //more than one accepted
			{
				tail.insert(tail.end(), ubf.begin() + length_header, ubf.end()); //move tail to next
				ubf.erase(ubf.begin() + length_header, ubf.end());
			}

			if (ubf.size() == length_header && length_header != 0) //exact needed packet
			{
				ph.Deserialize(ubf);
                if (ph.packet && ph.packet->id == 4)
				{
					std::shared_ptr<PacketResponseSector> prs = std::static_pointer_cast<PacketResponseSector>(ph.packet);
					RemoteClient::instance().ready[prs->s->offset] = prs->s;
				}
                if (ph.packet && ph.packet->id == 3)
                {
                    std::shared_ptr<PacketResponsePlayers> prp = std::static_pointer_cast<PacketResponsePlayers>(ph.packet);
                    temp_emp = prp->emp;
                }
				ubf.clear();

				if (!tail.empty()) //push tail to next
				{
					ubf.insert(ubf.end(), tail.begin(), tail.end());
					length_header = getLength(ubf);
					if (length_header == 0)
						LOG(info) << "hz chto, stream skipped";
					tail.clear();
				}
			}
			else
			{
				tail.clear();
				ubf.clear();
				LOG(error) << "broken stream =(";
			}
		}
    }
    catch(...)
    {
		ubf.clear();
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
