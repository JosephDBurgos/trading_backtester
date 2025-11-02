#pragma once
#include <string>
#include <vector>
#include <memory>
#include "price_bar.h"

namespace bt {

class IDataSource {
public:
    virtual ~IDataSource() = default;

    // timeframe is a free string for now ("1D", "1H", etc.)
    virtual std::vector<PriceBar> load(const std::string& symbol,
                                       const std::string& timeframe) = 0;
};

using DataSourcePtr = std::unique_ptr<IDataSource>;

} // namespace bt

