#pragma once
#include <string>
#include "data_source.h"

namespace bt {

class CsvDataSource : public IDataSource {
public:
    explicit CsvDataSource(std::string rootFolder);
    std::vector<PriceBar> load(const std::string& symbol,
                               const std::string& timeframe) override;

private:
    std::string root_;
    static std::chrono::system_clock::time_point parseTimestamp(const std::string& s);
};

} // namespace bt

