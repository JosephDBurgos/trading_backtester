#pragma once
#include <string>

namespace bt {

class Metrics {
public:
    void computeFromFile(const std::string& filename = "trades.csv");
};

} // namespace bt

