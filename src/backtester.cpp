#include "schema/backtester.h"
#include "schema/price_feed.h"
#include "schema/strategy.h"
#include "schema/trade_logger.h"
#include "schema/portfolio.h"
#include "schema/metrics.h"
#include <chrono>
#include <iostream>

namespace bt {

void Backtester::run(PriceFeed& feed, Strategy& strategy,
                     const std::string& strategyName,
                     const std::string& symbol,
                     const std::string& timeframe) {
    using namespace std::chrono;

    auto t0 = high_resolution_clock::now();

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
    } // logger + portfolio flushed/closed here

    auto t1 = high_resolution_clock::now();
    long long runMs = duration_cast<milliseconds>(t1 - t0).count();

    std::cout << "[Backtester] " << strategyName
              << " on " << symbol
              << " completed in " << runMs << " ms.\n";

    // --- Post-Backtest Metrics (+ runtime) ---
    std::string startDate = feed.getStartDate();
    std::string endDate   = feed.getEndDate();

    Metrics metrics;
    metrics.computeFromFile("trades.csv",
                        strategyName,
                        symbol,
                        timeframe,
                        runMs,
                        startDate,
                        endDate);

    }
} // namespace bt

