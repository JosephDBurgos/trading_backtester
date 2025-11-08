#pragma once
#include <string>

namespace bt {

class Metrics {
public:
    void computeFromFile(const std::string& filename,
                        const std::string& strategyName,
                        const std::string& symbol,
                        const std::string& timeframe);
};

} // namespace bt

