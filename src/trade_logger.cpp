#include "schema/trade_logger.h"
#include <fstream>

namespace bt {

TradeLogger::TradeLogger(const std::string& filename) {
    file_.open(filename);
    if (file_.is_open()) {
        file_ << "date,symbol,action,price,quantity\n";
    }
}

TradeLogger::~TradeLogger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void TradeLogger::logTrade(const std::string& date,
                           const std::string& symbol,
                           const std::string& action,
                           double price,
                           int quantity) {
    if (file_.is_open()) {
        file_ << date << "," << symbol << "," << action << ","
              << price << "," << quantity << "\n";
    }
}

} // namespace bt

