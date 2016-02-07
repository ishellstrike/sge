/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES
#define GLM_SWIZZLE

#define CLIENT

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gamewindow.h"

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#include <boost/exception/diagnostic_information.hpp>
#include "boost_log.hpp"
#include "prefecences.h"

#ifdef _MSC_VER
#ifndef _DEBUG
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif

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
			if (strstr(argv[i], ("-p")) != 0)
			{
				sscanf(argv[i]+2, "%d", &Prefecences::Instance()->server_port);
			}

			if (strstr(argv[i], ("-a")) != 0)
			{
				Prefecences::Instance()->server_adress = "255.255.255.255";
				sscanf(argv[i]+2, "%s", Prefecences::Instance()->server_adress.c_str());
			}
        }

    boost::log::add_common_attributes();
    //std::shared_ptr< Hive > hive( new Hive() );
    //std::shared_ptr<MyConnection> mc( new MyConnection(hive));

    try {
        GameWindow gw;
        gw.Mainloop();
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
