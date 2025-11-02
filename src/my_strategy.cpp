#include "schema/strategy.h"
#include <iostream>

namespace bt {

class MyStrategy : public Strategy {
public:
    void onBar(const PriceBar& bar, Portfolio& portfolio) override {
        static int count = 0;
        if (count == 0) {
            portfolio.buy("AAPL", bar.close, 10);
        } else if (count == 2) {
            portfolio.sell("AAPL", bar.close, 10);
        }
        count++;

        std::cout << "[MyStrategy] " << bar.toString() << "\n";
    }
};

Strategy* makeMyStrategy() { return new MyStrategy(); }

} // namespace bt

