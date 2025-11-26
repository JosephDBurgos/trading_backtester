#include "schema/metrics.h"
#include "schema/results_db.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <filesystem>
#include <chrono>

namespace bt {

struct Trade {
    std::string date, symbol, action;
    double price = 0.0;
    int quantity = 0;
};

void Metrics::computeFromFile(const std::string& filename,
                              const std::string& strategyName,
                              const std::string& symbol,
                              const std::string& timeframe,
                              long long runMs,
                              const std::string& startDate,
                              const std::string& endDate) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[Metrics] Could not open " << filename << "\n";
        return;
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cout << "[Metrics] File is empty.\n";
        return;
    }
    if (!line.empty() && line.back() == '\r') line.pop_back();

    std::vector<Trade> trades;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::stringstream ss(line);
        Trade t;
        std::string priceStr, qtyStr;

        std::getline(ss, t.date, ',');
        std::getline(ss, t.symbol, ',');
        std::getline(ss, t.action, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, qtyStr);

        auto trim = [](std::string& s) {
            while (!s.empty() && (s.back() == '\r' || s.back() == '\n' || s.back() == ' '))
                s.pop_back();
            while (!s.empty() && s.front() == ' ')
                s.erase(0, 1);
        };
        trim(t.date);
        trim(t.symbol);
        trim(t.action);
        trim(priceStr);
        trim(qtyStr);

        try {
            t.price = std::stod(priceStr);
            t.quantity = std::stoi(qtyStr);
        } catch (...) {
            std::cerr << "[Metrics] Skipping malformed line: " << line << "\n";
            continue;
        }

        trades.push_back(t);
    }

    if (trades.empty()) {
        std::cout << "[Metrics] No trades found.\n";
        return;
    }

    double totalPnL = 0.0;
    int wins = 0;
    int losses = 0;
    int tradeCount = 0;

    for (size_t i = 0; i + 1 < trades.size(); i++) {
        const auto& buy = trades[i];
        const auto& sell = trades[i + 1];

        if (buy.action == "BUY" && sell.action == "SELL" && buy.symbol == sell.symbol) {
            double pnl = (sell.price - buy.price) * buy.quantity;
            totalPnL += pnl;
            tradeCount++;
            if (pnl > 0)
                wins++;
            else
                losses++;
        }
    }

    double winRate = (tradeCount > 0) ? (100.0 * wins / tradeCount) : 0.0;
    double avgPnL  = (tradeCount > 0) ? (totalPnL / tradeCount) : 0.0;

    std::cout << "\n========== Backtest Metrics ==========\n";
    std::cout << "Total Trades:   " << tradeCount << "\n";
    std::cout << "Winning Trades: " << wins << "\n";
    std::cout << "Losing Trades:  " << losses << "\n";
    std::cout << "Win Rate:       " << winRate << "%\n";
    std::cout << "Average PnL:    $" << avgPnL << "\n";
    std::cout << "Total PnL:      $" << totalPnL << "\n";
    std::cout << "=====================================\n\n";

    // ===== Persist Results into SQLite Database =====
    try {
        bt::ResultsDB db("results/results.db");

        bt::RunResult result;
        result.strategy  = strategyName; // ✅ dynamic
        result.symbol    = symbol;       // ✅ dynamic
        result.timeframe = timeframe;    // ✅ dynamic
        result.start     = startDate; // placeholder for now
        result.end       = endDate; // placeholder for now
        result.trades    = tradeCount;
        result.winRate   = winRate;
        result.totalPnL  = totalPnL;
        result.avgPnL    = avgPnL;
        result.maxDD     = 0.0;
        result.sharpe    = 0.0;
        result.commit    = "";
        result.runMs     = runMs;

        db.upsert(result);

        std::cout << "[Metrics] Results saved to results/results.db\n";
    } catch (const std::exception& e) {
        std::cerr << "[Metrics] Failed to write results: " << e.what() << "\n";
    }
}

} // namespace bt

