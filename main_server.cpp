/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES
#define GLM_SWIZZLE
#define WIN32_LEAN_AND_MEAN

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <logger.h>
#include <prefecences.h>

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#include "boost_log.hpp"

#include <core/db.h>
#include "network.h"

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <mutex>

#include <core/network/packetholder.h>
#include <glm/gtx/string_cast.hpp>
#include "core/network/remoteserver.h"

#include "core/sector.h"
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <sstream>

class ServerConnection : public Connection
{
private:
    void OnAccept( const std::string & host, uint16_t port )
    {
        LOG(info) << "[" << __FUNCTION__ << "] " << host << ":" << port;

        // Start the next receive
        Recv();
    }

    void OnConnect( const std::string & host, uint16_t port )
    {
        LOG(info) << "[" << __FUNCTION__ << "] " << host << ":" << port;

        // Start the next receive
        Recv();
    }

    void OnSend( const std::vector< uint8_t > & buffer )
    {
    }

    void OnRecv( std::vector< uint8_t > & buffer )
    {
        // Start the next receive
        Recv();

        PacketHolder ph;

        try {
            ph.Deserialize(buffer);
        } catch (...)
        {
            return;
        }

        if(ph.packet->id == Packet::TidFor<PacketRequestSector>())
        {
            PacketRequestSector &prs = *std::static_pointer_cast<PacketRequestSector>(ph.packet);

            std::shared_ptr<Sector> s = RemoteServer::instance().GetSector(prs.pos);
            if(!s)
                return;

            PacketHolder ph;
            ph.Init<PacketResponseSector>(s);

            Send(ph);
        }
    }

    void OnTimer( const boost::posix_time::time_duration & delta )
    {
        //LOG(info) << "[" << __FUNCTION__ << "] " << delta;
    }

    void OnError( const boost::system::error_code & error )
    {
        LOG(info) << "[" << __FUNCTION__ << "] " << error;
    }

public:
    ServerConnection( std::shared_ptr< Hive > hive )
        : Connection( hive )
    {
    }

    ~ServerConnection()
    {
    }
};

class MyAcceptor : public Acceptor
{
private:
    bool OnAccept( std::shared_ptr< Connection > connection, const std::string & host, uint16_t port )
    {
        LOG(info) << "[" << __FUNCTION__ << "] " << host << ":" << port;

        return true;
    }

    void OnTimer( const boost::posix_time::time_duration & delta )
    {
        //LOG(info) << "[" << __FUNCTION__ << "] " << delta;
    }

    void OnError( const boost::system::error_code & error )
    {
        LOG(info) << "[" << __FUNCTION__ << "] " << error;
    }

public:
    MyAcceptor( std::shared_ptr< Hive > hive )
        : Acceptor( hive )
    {
    }

    ~MyAcceptor()
    {
    }
};
int main(int argc, char** argv)
{
    boost::log::add_console_log(
            std::cout,
            boost::log::keywords::format = (
                boost::log::expressions::stream
                    << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
                    << " "
                    << boost::log::trivial::severity
                    << " "
                    << boost::log::expressions::message
            )
    );

    boost::log::add_file_log
    (
        boost::log::keywords::file_name = "jarg_%5N.log",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::format = (
            boost::log::expressions::stream
                << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
                << " "
                << boost::log::trivial::severity
                << " "
                << boost::log::expressions::message
        )
    );

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );

    int port = 7777;

    if(argc >= 2)
        for(int i = 1; i < argc; i++)
        {
            if(strcmp(argv[i], ("-nosound")) == 0)
                Prefecences::Instance()->no_sound = true;

            if(strcmp(argv[i], ("-t")) == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::trace
                );
            if(strcmp(argv[i], ("-d")) == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::debug
                );
            if(strcmp(argv[i], ("-i")) == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::info
                );
            if(strcmp(argv[i], ("-w")) == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::warning
                );
            if(strcmp(argv[i], ("-e")) == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::error
                );
            if(strcmp(argv[i], ("-f")) == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::fatal
                );
            if(strstr(argv[i], ("-p")) != 0)
            {
                sscanf(argv[i], "-p%d", &port);
            }
        }

    boost::log::add_common_attributes();


    try {
        DB::Load();

        RemoteServer::instance();
        auto clie = [](){
            while(true)
            {
                RemoteServer::instance().Process();
                boost::this_thread::sleep_for( boost::chrono::milliseconds(50) );
            }
        };
        boost::thread th(clie);
        th.detach();

        LOG(info) << "Server ready";

        std::shared_ptr< Hive > hive( new Hive() );

        std::shared_ptr< MyAcceptor > acceptor( new MyAcceptor( hive ) );
        LOG(info) << "listening on port " << port;
        acceptor->Listen( "127.0.0.1", port );

        std::vector<std::shared_ptr< ServerConnection >> connection(10);
        for(int i = 0; i < 10; i++)
        {
            connection[i] = std::shared_ptr<ServerConnection>(new ServerConnection( hive ));
            acceptor->Accept( connection[i] );
        }

        while( true )
        {
            hive->Poll();
            Sleep( 1 );
        }

        hive->Stop();
    }
    catch (boost::exception &e)
    {
        LOG(fatal) << boost::diagnostic_information(e);
        throw;
    }
    catch (std::exception &e)
    {
        LOG(fatal) << e.what();
        throw;
    }

    return 0;
}

//------------------------------------------------------------------------------
