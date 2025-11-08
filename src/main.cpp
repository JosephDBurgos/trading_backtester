#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <filesystem>

#include "schema/csv_data_source.h"
#include "schema/data_loader.h"
#include "schema/price_feed.h"
#include "schema/backtester.h"
#include "schema/strategy.h"

namespace bt { Strategy* makeMyStrategy(); } // from my_strategy.cpp

int main(int argc, char* argv[]) {
    std::string strategyName = "my_strategy";
    std::string timeframe = "1D";
    std::vector<std::string> symbols = {"AAPL"}; // default if none passed

    // --- Parse CLI arguments ---
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--strategy" && i + 1 < argc)
            strategyName = argv[++i];
        else if (arg == "--symbol" && i + 1 < argc)
            symbols = {argv[++i]};
        else if (arg == "--symbols" && i + 1 < argc) {
            symbols.clear();
            // comma-separated list
            std::string list = argv[++i];
            size_t start = 0, end;
            while ((end = list.find(',', start)) != std::string::npos) {
                symbols.push_back(list.substr(start, end - start));
                start = end + 1;
            }
            symbols.push_back(list.substr(start));
        }
        else if (arg == "--timeframe" && i + 1 < argc)
            timeframe = argv[++i];
    }

    std::cout << "[Backtester] Running " << strategyName
              << " on " << symbols.size()
              << " symbol(s) (" << timeframe << ")\n";

    for (const auto& symbol : symbols) {
        try {
            std::cout << "\n=== Running " << strategyName << " on " << symbol << " ===\n";
            auto source = std::make_unique<bt::CsvDataSource>("../data");
            bt::DataLoader loader(std::move(source));
            auto bars = loader.loadBars(symbol, timeframe);

            if (bars.empty()) {
                std::cerr << "[Error] No data for " << symbol << "\n";
                continue;
            }

            bt::PriceFeed feed(std::move(bars));
            std::unique_ptr<bt::Strategy> strat(bt::makeMyStrategy());
            bt::Backtester engine;
            engine.run(feed, *strat, strategyName, symbol, timeframe);

            std::cout << "[Backtester] Finished " << symbol << "\n";

        } catch (const std::exception& e) {
            std::cerr << "[Fatal] " << e.what() << "\n";
        }
    }

    return 0;
}

