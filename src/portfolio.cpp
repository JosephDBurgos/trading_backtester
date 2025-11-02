#include "schema/portfolio.h"
#include <iostream>

namespace bt {

Portfolio::Portfolio(double starting_cash)
    : cash_(starting_cash), position_(0), last_price_(0.0) {}

void Portfolio::buy(const std::string& symbol, double price, int quantity) {
    double cost = price * quantity;
    if (cost > cash_) {
        std::cout << "[Portfolio] Not enough cash to buy!\n";
        return;
    }

    cash_ -= cost;
    position_ += quantity;
    last_price_ = price;

    std::cout << "[Portfolio] Bought " << quantity << " shares of "
              << symbol << " @ " << price << "\n";
}

void Portfolio::sell(const std::string& symbol, double price, int quantity) {
    if (quantity > position_) {
        std::cout << "[Portfolio] Not enough shares to sell!\n";
        return;
    }

    double proceeds = price * quantity;
    cash_ += proceeds;
    position_ -= quantity;
    last_price_ = price;

    std::cout << "[Portfolio] Sold " << quantity << " shares of "
              << symbol << " @ " << price << "\n";
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

