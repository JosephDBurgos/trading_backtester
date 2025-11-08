#include "schema/strategy.h"
#include "schema/indicator.h"
#include "schema/time_utils.h"
#include <iostream>

namespace bt {

class MyStrategy : public Strategy {
private:
    SimpleMovingAverage shortMA_{3};
    SimpleMovingAverage longMA_{5};
    RelativeStrengthIndex rsi_{14};
    bool inPosition_ = false;

public:
    void onBar(const PriceBar& bar, Portfolio& portfolio) override {
        shortMA_.update(bar);
        longMA_.update(bar);
        rsi_.update(bar);

        std::string date = toDateString(bar.timestamp);

        double shortVal = shortMA_.value();
        double longVal  = longMA_.value();
        double rsiVal   = rsi_.value();

        if (shortVal == 0.0 || longVal == 0.0 || rsiVal == 0.0)
            return;

        // Buy when SMA cross up + RSI confirms not overbought
        if (!inPosition_ && shortVal > longVal && rsiVal < 70) {
            portfolio.buy("AAPL", bar.close, 10, date);
            inPosition_ = true;
        }
        // Sell when SMA cross down + RSI confirms not oversold
        else if (inPosition_ && shortVal < longVal && rsiVal > 30) {
            portfolio.sell("AAPL", bar.close, 10, date);
            inPosition_ = false;
        }
    }
};

Strategy* makeMyStrategy() { return new MyStrategy(); }

} // namespace bt

