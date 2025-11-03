#include "schema/portfolio.h"
#include <iostream>

namespace bt {

Portfolio::Portfolio(double starting_cash, TradeLogger* logger)
    : cash_(starting_cash), position_(0), last_price_(0.0), logger_(logger) {}

void Portfolio::buy(const std::string& symbol, double price, int quantity, const std::string& date) {
    double cost = price * quantity;
    if (cost > cash_) {
        std::cout << "[Portfolio] Not enough cash to buy!\n";
        return;
    }

    cash_ -= cost;
    position_ += quantity;
    last_price_ = price;

    if (logger_) {
        logger_->logTrade(date, symbol, "BUY", price, quantity);
    }
}

void Portfolio::sell(const std::string& symbol, double price, int quantity, const std::string& date) {
    if (quantity > position_) {
        std::cout << "[Portfolio] Not enough shares to sell!\n";
        return;
    }

    double proceeds = price * quantity;
    cash_ += proceeds;
    position_ -= quantity;
    last_price_ = price;


    if (logger_) {
        logger_->logTrade(date, symbol, "SELL", price, quantity);
    }
}

double Portfolio::equity() const {
    return cash_ + position_ * last_price_;
}

void Portfolio::summary() const {
    std::cout << "[Portfolio] Cash: " << cash_
              << " | Position: " << position_
              << " | Equity: " << equity() << "\n";
}

} // namespace bt

