#pragma once
#include "price_feed.h"
#include "strategy.h"

namespace bt {

class Backtester {
public:
    void run(PriceFeed& feed, Strategy& strategy) {
        while (feed.hasNext()) {
            strategy.onBar(feed.next());
        }
    }
};

} // namespace bt

