#include "logger.h"
#include <iostream>
#include <ctime>
#include <memory>
#include <thread>
#include "helper.h"
#include <ostream>
#include <exception>

Log::Log(int type, const char *file, int line) :
    o_stream(std::cout.rdbuf()),
    m_type(type)
{
    if(type < max_level)
    {
        return;
    }

    switch (m_type) {
    case verbose:
        o_stream << "V";
        break;
    case info:
        o_stream << "I";
        break;
    case error:
        o_stream << "E";
        break;
    case fatal:
        o_stream << "F";
        break;
    default:
        o_stream << "?";
        break;
    }

    std::thread::id id = std::this_thread::get_id();

    o_stream << " " << string_format("%6gms %16s %3d] ", clock()/static_cast<float>(CLOCKS_PER_SEC/1000), file, line).c_str();
}

Log::~Log()
{
    if(m_type < max_level)
    {
    }
    else
    {
        o_stream << std::endl;
    }

    if(m_type == fatal)
    {
        throw;
    }
}

LogLevel Log::max_level = info;
