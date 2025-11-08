#pragma once
#include "price_feed.h"
#include "strategy.h"

namespace bt {

class Backtester {
public:
    void run(PriceFeed& feed, Strategy& strategy,
            const std::string& strategyName,
            const std::string& symbol,
            const std::string& timeframe);
    
    };

} // namespace bt

