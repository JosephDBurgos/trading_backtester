#pragma once
#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>

namespace bt {

inline std::string toDateString(const std::chrono::system_clock::time_point& tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::ostringstream ss;
    ss << std::put_time(std::gmtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

} // namespace bt

