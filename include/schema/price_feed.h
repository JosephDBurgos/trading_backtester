#pragma once
#include <vector>
#include <cstddef>
#include <chrono>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "price_bar.h"

namespace bt {

class PriceFeed {
public:
    explicit PriceFeed(std::vector<PriceBar> bars)
    : bars_(std::move(bars)) {}

    bool hasNext() const { return idx_ < bars_.size(); }
    const PriceBar& next() { return bars_[idx_++]; }
    const PriceBar* peek() const { return hasNext() ? &bars_[idx_] : nullptr; }
    std::size_t size() const { return bars_.size(); }

    // ⭐ Convert timestamp → "YYYY-MM-DD"
    std::string formatDate(const std::chrono::system_clock::time_point& ts) const {
        std::time_t t = std::chrono::system_clock::to_time_t(ts);
        std::tm* tm = std::gmtime(&t);

        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
        return std::string(buf);
    }

    // ⭐ Real start date
    std::string getStartDate() const {
        return bars_.empty() ? "" : formatDate(bars_.front().timestamp);
    }

    // ⭐ Real end date
    std::string getEndDate() const {
        return bars_.empty() ? "" : formatDate(bars_.back().timestamp);
    }

private:
    std::vector<PriceBar> bars_;
    std::size_t idx_ = 0;
};

} // namespace bt

