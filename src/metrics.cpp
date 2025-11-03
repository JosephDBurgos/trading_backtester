#include "schema/metrics.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <filesystem>

namespace bt {

struct Trade {
    std::string date, symbol, action;
    double price = 0.0;
    int quantity = 0;
};

void Metrics::computeFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[Metrics] Could not open " << filename << "\n";
        return;
    }

    //std::cout << "[Metrics] CWD: " << std::filesystem::current_path() << "\n";
    //std::cout << "[Metrics] Trying to read: " 
    //      << std::filesystem::absolute(filename) << "\n";
    //std::cout << "[Metrics] Successfully opened " << filename << "\n";

    std::string line;
    // Read and normalize header
    if (!std::getline(file, line)) {
        std::cout << "[Metrics] File is empty.\n";
        return;
    }
    if (!line.empty() && line.back() == '\r') line.pop_back(); // normalize header


    std::vector<Trade> trades;


    while (std::getline(file, line)) {

        //std::cout << "[Metrics] Reading line: '" << line << "'\n";
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::stringstream ss(line);
        Trade t;
        std::string priceStr, qtyStr;

        // Read up to commas
        std::getline(ss, t.date, ',');
        std::getline(ss, t.symbol, ',');
        std::getline(ss, t.action, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, qtyStr); // <-- FIXED: read rest of line (no trailing comma)

    // Trim spaces and carriage returns
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

    // Simple pairing (BUY -> SELL)
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
    double avgPnL = (tradeCount > 0) ? (totalPnL / tradeCount) : 0.0;

    std::cout << "\n========== Backtest Metrics ==========\n";
    std::cout << "Total Trades:   " << tradeCount << "\n";
    std::cout << "Winning Trades: " << wins << "\n";
    std::cout << "Losing Trades:  " << losses << "\n";
    std::cout << "Win Rate:       " << winRate << "%\n";
    std::cout << "Average PnL:    $" << avgPnL << "\n";
    std::cout << "Total PnL:      $" << totalPnL << "\n";
    std::cout << "=====================================\n\n";
}

} // namespace bt

