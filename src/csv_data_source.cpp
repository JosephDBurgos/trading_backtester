#include "schema/csv_data_source.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace bt {

CsvDataSource::CsvDataSource(std::string rootFolder)
: root_(std::move(rootFolder)) {}

std::chrono::system_clock::time_point CsvDataSource::parseTimestamp(const std::string& s) {
    std::tm tm{};
    tm.tm_isdst = -1;
    std::istringstream iss(s);

    // Handle "YYYY-MM-DD" or "YYYY-MM-DD HH:MM:SS"
    if (s.size() <= 10) {
        iss >> std::get_time(&tm, "%Y-%m-%d");
        // Default to midnight if no time provided
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
    } else {
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    }

    if (iss.fail()) {
        throw std::runtime_error("Failed to parse timestamp: " + s);
    }

#if defined(_WIN32)
    std::time_t tt = _mkgmtime(&tm);
#else
    std::time_t tt = timegm(&tm);
#endif
    return std::chrono::system_clock::from_time_t(tt);
}


std::vector<PriceBar> CsvDataSource::load(const std::string& symbol,
                                          const std::string& timeframe) {
    // Convention: <root>/<SYMBOL>_<TIMEFRAME>.csv  e.g., data/AAPL_1D.csv
    const std::string path = root_ + "/" + symbol + "_" + timeframe + ".csv";

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("CsvDataSource: cannot open file: " + path);
    }

    std::vector<PriceBar> out;
    out.reserve(1024);

    std::string line;
    // Expect header: Date,Open,High,Low,Close,Volume
    if (!std::getline(file, line)) return out;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string dateStr, openStr, highStr, lowStr, closeStr, volStr;

        // Simple CSV splitting (no quoted fields handling)
        std::getline(ss, dateStr, ',');
        std::getline(ss, openStr, ',');
        std::getline(ss, highStr, ',');
        std::getline(ss, lowStr,  ',');
        std::getline(ss, closeStr,',');
        std::getline(ss, volStr,  ',');

        PriceBar bar;
        bar.timestamp = parseTimestamp(dateStr);
        bar.open  = std::stod(openStr);
        bar.high  = std::stod(highStr);
        bar.low   = std::stod(lowStr);
        bar.close = std::stod(closeStr);
        bar.volume= std::stod(volStr);
        out.push_back(bar);
    }

    return out;
}

} // namespace bt

