#pragma once
#include "price_bar.h"

namespace bt {

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual void onBar(const PriceBar& bar) = 0;
};

} // namespace bt

