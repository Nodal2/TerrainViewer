#ifndef MESHTOOL_UTIL
#define MESHTOOL_UTIL

#include <iostream>
#include <fstream>

namespace util
{
    bool read_file(const std::string &path, std::string &content);
    
    template<typename T>
    T clamp(const T &value, const T &low, const T &high);
}

template<typename T>
T util::clamp(const T &value, const T &low, const T &high)
{
    return value <= low ? low : value >= high ? high : value;
}

#endif