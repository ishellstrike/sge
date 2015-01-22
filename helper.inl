#pragma once
#define _USE_MATH_DEFINES
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <string>
#include <stdarg.h>
#include <math.h>
#include "colorextender.h"

#ifdef NDEBUG
#define OPENGL_CHECK_ERRORS() \
    while( unsigned int openGLError = glGetError()) \
    { \
    LOG(error) << "OpenGL Error " << openGLError<< " -- " << glewGetErrorString(openGLError); \
    };
#else
#define OPENGL_CHECK_ERRORS() ((void)0)
#endif // NDEBUG

inline glm::vec4 lerp(glm::vec4 &a, glm::vec4 &b, float a_percent){
    return a_percent * a + (1.0f - a_percent) * b;
}

inline glm::vec3 lerp(glm::vec3 &a, glm::vec3 &b, float a_percent){
    return a_percent * a + (1.0f - a_percent) * b;
}

inline std::string wstring_to_string(const std::wstring &wstr)
{
    std::string s(wstr.length(), ' ');
    std::copy(wstr.begin(), wstr.end(), s.begin());
    return s;
}

inline std::wstring string_to_wstring(const std::string &str)
{
    std::wstring s(str.length(), L' ');
    std::copy(str.begin(), str.end(), s.begin());
    return s;
}

inline double triangular(double min, double max, double mean) {
    double U = rand() / (double) RAND_MAX;
    double F = (mean - min) / (max - min);
    if (U <= F)
        return min + sqrt(U * (max - min) * (mean - min));
    else
        return max - sqrt((1 - U) * (max - min) * (max - mean));
}

inline double uniform(double min, double max) {
    double U = rand() / (double) RAND_MAX;
    return min + U*(max - min);
}

inline double triangular(double min, double max) {
    double U = rand() / (double) RAND_MAX;
    double mean = (max + min) / 2.0;
    double F = (mean - min) / (max - min);
    if (U <= F)
        return min + sqrt(U * (max - min) * (mean - min));
    else
        return max - sqrt((1 - U) * (max - min) * (max - mean));
}

inline double triangular() {
    double U = rand() / (double) RAND_MAX;
    static const double mean = 0.5;
    static const double max = 1;
    static const double min = 0;
    double F = (mean - min) / (max - min);
    if (U <= F)
        return min + sqrt(U * (max - min) * (mean - min));
    else
        return max - sqrt((1 - U) * (max - min) * (max - mean));
}

inline bool inLimsV(glm::vec2 param1, glm::vec2 wpos, glm::vec2 size)
{
    return param1.x >= wpos.x && param1.y >= wpos.y && param1.x <= size.x && param1.y <= size.y;
}

template<class _Ty>
inline bool inLims(_Ty param1, _Ty wpos, _Ty size)
{
    return param1 >= wpos && param1 <= size;
}

inline char keyToChar(int key, bool shift){
    int r = 0;
    if(inLims(key, 48, 57)){
        r = '0'+key-48;
    }
    if(inLims(key, 65, 90)){
        r = 'a'+key-65;
    }

    if(shift)
        return toupper(r);
    return r;
}

inline std::string string_format(const std::string fmt_str, ...) {
    int n = ((int)fmt_str.size()) * 2;
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]);
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        int final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}

inline std::string to_traf_string(double traf){
    if (traf > 1024*1024*1024) {
        return string_format("%2f GiB", traf/(1024.0*1024.0*1024.0));
    }
    if (traf > 1024 * 1024)
    {
        return string_format("%2f MiB", traf / (1024.0 * 1024.0));
    }
    if (traf > 1024)
    {
        return string_format("%2f KiB", traf / 1024.0);
    }
    return string_format("%i B", (int)traf);
}

namespace std
{
    inline std::string to_string(const glm::vec2& a){
        return string_format("{%g, %g, %g}", a.x, a.y);
    }
    inline std::string to_string(const glm::vec3& a){
        return string_format("{%g, %g, %g}", a.x, a.y, a.z);
    }
    inline std::string to_string(const glm::vec4& a){
        return string_format("{%g, %g, %g, %g}", a.x, a.y, a.z, a.w);
    }

    inline std::string to_string(const glm::quat& a){
        return string_format("{%g, %g, %g, %g}", a.x, a.y, a.z, a.w);
    }

    inline std::string to_string(const glm::mat3& a){
        return string_format("{%g, %g, %g}\n{%g, %g, %g}\n{%g, %g, %g}", a[0][0], a[1][0], a[2][0], a[0][1], a[1][1], a[2][1], a[0][2], a[1][2], a[2][2]);
    }
    inline std::string to_string(const glm::mat4& a){
        return string_format("{%g, %g, %g, %g}\n{%g, %g, %g, %g}\n{%g, %g, %g, %g}", a[0][0], a[1][0], a[2][0], a[3][0], a[0][1], a[1][1], a[2][1], a[3][1], a[0][2], a[1][2], a[2][2], a[3][2], a[0][3], a[1][3], a[2][3], a[3][3]);
    }
}
