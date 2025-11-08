#include "schema/backtester.h"
#include "schema/price_feed.h"
#include "schema/strategy.h"
#include "schema/trade_logger.h"
#include "schema/portfolio.h"
#include "schema/metrics.h"

namespace bt {

void Backtester::run(PriceFeed& feed, Strategy& strategy,
                    const std::string& strategyName,
                    const std::string& symbol,
                    const std::string& timeframe) {
    {
        // --- Trading Session Context ---
        TradeLogger logger("trades.csv");
        Portfolio portfolio(100000.0, &logger);

        // --- Backtesting Loop ---
        while (feed.hasNext()) {
            const auto& bar = feed.next();
            strategy.onBar(bar, portfolio);
        }

        // --- Portfolio Summary ---
        portfolio.summary();

    } // <-- logger and portfolio automatically destroyed here (file closed, data flushed)

    // --- Post-Backtest Metrics ---
    Metrics metrics;
    metrics.computeFromFile("trades.csv", strategyName, symbol, timeframe);
}

} // namespace bt

