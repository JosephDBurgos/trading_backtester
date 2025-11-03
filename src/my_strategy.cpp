#include "schema/strategy.h"
#include "schema/indicator.h"
#include "schema/time_utils.h"
#include <iostream>

namespace bt {

class MyStrategy : public Strategy {
private:
    SimpleMovingAverage shortMA_{3};   // short-term moving average
    SimpleMovingAverage longMA_{5};    // long-term moving average
    bool inPosition_ = false;

public:
    void onBar(const PriceBar& bar, Portfolio& portfolio) override {
        // Update indicators with new price bar
        shortMA_.update(bar);
        longMA_.update(bar);

        // Convert timestamp to readable date string
        std::string date = toDateString(bar.timestamp);

        // Compute current values
        double shortVal = shortMA_.value();
        double longVal  = longMA_.value();


        // Only trade when both MAs have enough data
        if (shortVal == 0.0 || longVal == 0.0)
            return;

        // Buy when short MA crosses above long MA
        if (!inPosition_ && shortVal > longVal) {
            portfolio.buy("AAPL", bar.close, 10, date);
            inPosition_ = true;
        }
        // Sell when short MA crosses below long MA
        else if (inPosition_ && shortVal < longVal) {
            portfolio.sell("AAPL", bar.close, 10, date);
            inPosition_ = false;
        }
    }
};

Strategy* makeMyStrategy() { return new MyStrategy(); }

} // namespace bt

