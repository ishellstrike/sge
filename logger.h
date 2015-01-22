#ifndef LOGGER_H
#define LOGGER_H
#include <ostream>
#include <string.h>
#include <stdarg.h>
#include <memory>

enum {
    info,
    error,
    fatal
};

#define LOG(level) Log(level, __FILE__, __LINE__)

class Log
{
public:
    Log(int type, const char *file, int line);
    ~Log();

    int m_type;

    std::ostream o_stream;

    template<typename T>
    Log &operator <<(const T &a)
    {
        o_stream << a;

        return *this;
    }
};

#endif // LOGGER_H
