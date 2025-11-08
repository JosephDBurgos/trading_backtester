#pragma once
#include "price_bar.h"
#include <deque>
#include <string>
#include <cmath>

namespace bt {

class Indicator {
public:
    virtual ~Indicator() = default;
    virtual void update(const PriceBar& bar) = 0;
    virtual double value() const = 0;
    virtual std::string name() const = 0;
};

// =============================
// Simple Moving Average (done)
// =============================
class SimpleMovingAverage : public Indicator {
private:
    std::deque<double> window_;
    size_t period_;
    double sum_;

public:
    explicit SimpleMovingAverage(size_t period);
    void update(const PriceBar& bar) override;
    double value() const override;
    std::string name() const override;
};

// =============================
// Relative Strength Index (RSI)
// =============================
class RelativeStrengthIndex : public Indicator {
private:
    size_t period_;
    std::deque<double> gains_, losses_;
    double lastClose_ = 0.0;
    double rsi_ = 0.0;
    bool initialized_ = false;

public:
    explicit RelativeStrengthIndex(size_t period);
    void update(const PriceBar& bar) override;
    double value() const override;
    std::string name() const override;
};

} // namespace bt

