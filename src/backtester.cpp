#include "schema/backtester.h"
#include "schema/portfolio.h"

namespace bt {

void Backtester::run(PriceFeed& feed, Strategy& strategy) {
    Portfolio portfolio(100000.0); // Starting balance

    while (feed.hasNext()) {
        const auto& bar = feed.next();
        strategy.onBar(bar, portfolio);
    }

    portfolio.summary();
}

} // namespace bt

