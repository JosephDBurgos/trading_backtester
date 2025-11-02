#pragma once
#include <string>

namespace bt {

class Portfolio {
private:
    double cash_;
    int position_;
    double last_price_;

public:
    Portfolio(double starting_cash = 100000.0);

    void buy(const std::string& symbol, double price, int quantity);
    void sell(const std::string& symbol, double price, int quantity);

    double equity() const;
    void summary() const;
};

} // namespace bt

