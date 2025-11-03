#include "schema/indicator.h"
#include <numeric>  

namespace bt {

SimpleMovingAverage::SimpleMovingAverage(size_t period)
    : period_(period), sum_(0.0) {}

void SimpleMovingAverage::update(const PriceBar& bar) {
    if (window_.size() == period_) {
        sum_ -= window_.front();
        window_.pop_front();
    }
    window_.push_back(bar.close);
    sum_ += bar.close;
}

double SimpleMovingAverage::value() const {
    if (window_.empty()) return 0.0;
    return sum_ / window_.size();
}

std::string SimpleMovingAverage::name() const {
    return "SMA(" + std::to_string(period_) + ")";
}

} // namespace bt

