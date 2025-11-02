#pragma once
#include <string>
#include "trade_logger.h"

namespace bt {

class Portfolio {
private:
    double cash_;
    int position_;
    double last_price_;
    TradeLogger* logger_;  // pointer, we don't own it

public:
    Portfolio(double starting_cash = 100000.0, TradeLogger* logger = nullptr);

    void buy(const std::string& symbol, double price, int quantity, const std::string& date = "");
    void sell(const std::string& symbol, double price, int quantity, const std::string& date = "");

    double equity() const;
    void summary() const;
};

} // namespace bt

