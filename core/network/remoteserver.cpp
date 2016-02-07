#include "remoteserver.h"
#include "core/levelgen.h"

RemoteServer::RemoteServer()
{

}

std::shared_ptr<Sector> RemoteServer::GetSector(const glm::ivec2 &v)
{
    auto f = ready.find( v );
    if( f != ready.end() )
    {
        std::pair<glm::ivec2, std::shared_ptr<Sector>> sec = *f;
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

void RemoteServer::Process()
{
    if( requested.size() )
    {
        auto r = requested.begin();
        auto a = std::shared_ptr<Sector>( new Sector( *r ) );
        Generate( *a );
        ready.insert( std::make_pair( *r, a ) );

        requested.erase( r );
    }
}

