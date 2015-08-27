/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef LOGGER_H
#define LOGGER_H
#include <ostream>
#include <string.h>
#include <stdarg.h>
#include <memory>

enum LogLevel {
    verbose,
    info,
    error,
    fatal
};

#define __SHORT_FILE2__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define __SHORT_FILE__ (strrchr(__SHORT_FILE2__, '\\') ? strrchr(__SHORT_FILE2__, '\\') + 1 : __SHORT_FILE2__)
#define LOG(level) Log(level, __SHORT_FILE__, __LINE__)

class Log
{
    int m_type;

    std::ostream o_stream;

public:
    Log(int type, const char *file, int line);
    ~Log();

    template<typename T>
    Log &operator <<(const T &a)
    {
        if(m_type < max_level)
        {
            return *this;
        }

        o_stream << a;

        return *this;
    }

    static LogLevel max_level;
};

#endif // LOGGER_H
