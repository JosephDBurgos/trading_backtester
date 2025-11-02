#include <iostream>
#include <memory>

#include "schema/csv_data_source.h"
#include "schema/data_loader.h"
#include "schema/price_feed.h"
#include "schema/backtester.h"
#include "schema/strategy.h"

namespace bt { Strategy* makeMyStrategy(); } // factory from my_strategy.cpp

int main() {
    try {
        // 1) Choose a source (CSV for now)
        auto source = std::make_unique<bt::CsvDataSource>("../data");

        // 2) Loader orchestrates loading
        bt::DataLoader loader(std::move(source));
        auto bars = loader.loadBars("AAPL", "1D");

        if (bars.empty()) {
            std::cerr << "No bars loaded.\n";
            return 1;
        }

        // 3) Feed to stream them
        bt::PriceFeed feed(std::move(bars));

        // 4) Strategy + Engine
        std::unique_ptr<bt::Strategy> strat(bt::makeMyStrategy());
        bt::Backtester engine;
        engine.run(feed, *strat);

        std::cout << "Processed " << feed.size() << " bars.\n";
    } catch (const std::exception& e) {
        std::cerr << "Fatal: " << e.what() << "\n";
        return 2;
    }
    return 0;
}

