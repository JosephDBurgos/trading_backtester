#include "schema/indicator.h"

namespace bt {

// ===== SMA =====
SimpleMovingAverage::SimpleMovingAverage(size_t period)
    : period_(period), sum_(0.0) {}

void SimpleMovingAverage::update(const PriceBar& bar) {
    double price = bar.close;
    window_.push_back(price);
    sum_ += price;

    if (window_.size() > period_) {
        sum_ -= window_.front();
        window_.pop_front();
    }
}

double SimpleMovingAverage::value() const {
    if (window_.size() < period_) return 0.0;
    return sum_ / window_.size();
}

std::string SimpleMovingAverage::name() const { return "SMA"; }

// ===== RSI =====
RelativeStrengthIndex::RelativeStrengthIndex(size_t period)
    : period_(period) {}

void RelativeStrengthIndex::update(const PriceBar& bar) {
    if (!initialized_) {
        lastClose_ = bar.close;
        initialized_ = true;
        return;
    }

    double change = bar.close - lastClose_;
    lastClose_ = bar.close;

    gains_.push_back(change > 0 ? change : 0.0);
    losses_.push_back(change < 0 ? -change : 0.0);

    if (gains_.size() > period_) gains_.pop_front();
    if (losses_.size() > period_) losses_.pop_front();

    if (gains_.size() == period_) {
        double avgGain = 0.0, avgLoss = 0.0;
        for (double g : gains_) avgGain += g;
        for (double l : losses_) avgLoss += l;
        avgGain /= period_;
        avgLoss /= period_;

        double rs = (avgLoss == 0) ? 100 : (avgGain / avgLoss);
        rsi_ = 100 - (100 / (1 + rs));
    }
}

double RelativeStrengthIndex::value() const { return rsi_; }

std::string RelativeStrengthIndex::name() const { return "RSI"; }

} // namespace bt

