/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES
#define GLM_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gamewindow.h"

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/detail/process_id.hpp>
#include <boost/log/detail/thread_id.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/format.hpp>

int main(int argc, char** argv)
{
    boost::log::add_console_log(
            std::cout,
            boost::log::keywords::format = (
                boost::log::expressions::stream
                    << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
                    << " ["
                    << boost::log::trivial::severity
                    << "] "
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
                << " ["
                << boost::log::trivial::severity
                << "] "
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
            if(strcmp(argv[i], "-t") == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::trace
                );
            if(strcmp(argv[i], "-d") == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::debug
                );
            if(strcmp(argv[i], "-i") == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::info
                );
            if(strcmp(argv[i], "-w") == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::warning
                );
            if(strcmp(argv[i], "-e") == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::error
                );
            if(strcmp(argv[i], "-f") == 0)
                boost::log::core::get()->set_filter
                (
                    boost::log::trivial::severity >= boost::log::trivial::fatal
                );
        }

    boost::log::add_common_attributes();

    try {
        GameWindow gw;
        gw.Mainloop();
    }
    catch( std::exception& e ) {
        LOG(error) << "Caught exception: " << e.what() << std::endl;
    }

    return 0;
}
//------------------------------------------------------------------------------





