#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

namespace bt {

struct PriceBar {
    std::chrono::system_clock::time_point timestamp;
    double open  = 0.0;
    double high  = 0.0;
    double low   = 0.0;
    double close = 0.0;
    double volume= 0.0;

    std::string toString() const {
        std::time_t t = std::chrono::system_clock::to_time_t(timestamp);
        std::ostringstream ss;

        // Print timestamp in UTC
        ss << std::put_time(std::gmtime(&t), "%Y-%m-%d %H:%M:%S");

        // Format prices with 2 decimal places
        ss << std::fixed << std::setprecision(2)
           << " | O:" << open
           << " H:" << high
           << " L:" << low
           << " C:" << close;

        // Format volume with no decimals, fixed (non-scientific)
        ss << " V:" << std::fixed << std::setprecision(0) << volume;

        return ss.str();
    }
};

} // namespace bt

