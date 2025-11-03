#pragma once
#include "price_bar.h"
#include <deque>
#include <string>

namespace bt {

class Indicator {
public:
    virtual ~Indicator() = default;
    virtual void update(const PriceBar& bar) = 0;
    virtual double value() const = 0;
    virtual std::string name() const = 0;
};

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

} // namespace bt

