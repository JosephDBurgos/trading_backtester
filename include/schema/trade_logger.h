#pragma once
#include <fstream>   // <-- add this
#include <string>

namespace bt {

class TradeLogger {
private:
    std::ofstream file_;

public:
    explicit TradeLogger(const std::string& filename = "trades.csv");
    ~TradeLogger();

    void logTrade(const std::string& date,
                  const std::string& symbol,
                  const std::string& action,
                  double price,
                  int quantity);
};

} // namespace bt

