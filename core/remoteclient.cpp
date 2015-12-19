#include "remoteclient.h"
#include "levelgen.h"
#include <thread>

RemoteClient::RemoteClient()
{

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
        ready.insert( std::make_pair( *r, std::move( a ) ) );

        requested.erase( r );
    }
}

