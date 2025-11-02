#include "schema/strategy.h"
#include <iostream>

namespace bt {

class MyStrategy : public Strategy {
public:
    void onBar(const PriceBar& bar) override {
        // placeholder: just print first few bars
        static int count = 0;
        if (count++ < 3) {
            std::cout << "[MyStrategy] " << bar.toString() << "\n";
        }
    }
};

// Expose a factory for main.cpp
Strategy* makeMyStrategy() { return new MyStrategy(); }

} // namespace bt

