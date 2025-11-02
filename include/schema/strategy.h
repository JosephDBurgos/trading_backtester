#pragma once
#include "price_bar.h"
#include "portfolio.h"

namespace bt {

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual void onBar(const PriceBar& bar, Portfolio& portfolio) = 0;
};

} // namespace bt

