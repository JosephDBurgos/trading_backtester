#include "schema/backtester.h"
#include "schema/price_feed.h"   // <── add this
#include "schema/strategy.h"     // <── add this
#include "schema/trade_logger.h" // already needed
#include "schema/portfolio.h"    // <── also needed if you use Portfolio

namespace bt {

void Backtester::run(PriceFeed& feed, Strategy& strategy) {
    TradeLogger logger("trades.csv");
    Portfolio portfolio(100000.0, &logger);

    while (feed.hasNext()) {
        const auto& bar = feed.next();
        strategy.onBar(bar, portfolio);
    }

    portfolio.summary();
}

} // namespace bt

