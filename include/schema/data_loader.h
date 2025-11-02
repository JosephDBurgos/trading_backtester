#pragma once
#include <memory>
#include <string>
#include <vector>
#include "data_source.h"

namespace bt {

class DataLoader {
public:
    explicit DataLoader(DataSourcePtr source)
    : source_(std::move(source)) {}

    std::vector<PriceBar> loadBars(const std::string& symbol,
                                   const std::string& timeframe);

private:
    DataSourcePtr source_;
};

} // namespace bt

