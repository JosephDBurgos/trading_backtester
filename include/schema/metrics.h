#pragma once
#include <string>

namespace bt {

class Metrics {
public:
    void computeFromFile(const std::string& filename,
                     const std::string& strategyName,
                     const std::string& symbol,
                     const std::string& timeframe,
                     long long runMs,
                     const std::string& startDate,
                     const std::string& endDate);
};

} // namespace bt

