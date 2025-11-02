#pragma once
#include <vector>
#include <cstddef>
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

private:
    std::vector<PriceBar> bars_;
    std::size_t idx_ = 0;
};

} // namespace bt

