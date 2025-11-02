#include "schema/strategy.h"
#include "schema/time_utils.h"
#include <iostream>

namespace bt {

class MyStrategy : public Strategy {
public:
    void onBar(const PriceBar& bar, Portfolio& portfolio) override {
        static int count = 0;

        // Convert chrono timestamp to readable string
        std::string date = toDateString(bar.timestamp);

        if (count == 0) {
            portfolio.buy("AAPL", bar.close, 10, date);
        } else if (count == 2) {
            portfolio.sell("AAPL", bar.close, 10, date);
        }

        count++;
        std::cout << "[MyStrategy] " << bar.toString() << "\n";
    }
};

Strategy* makeMyStrategy() { return new MyStrategy(); }

} // namespace bt

