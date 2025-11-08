#pragma once
#include <string>

namespace bt {

struct RunResult {
    std::string strategy, symbol, timeframe, start, end;
    int trades = 0;
    double winRate = 0.0, totalPnL = 0.0, avgPnL = 0.0, maxDD = 0.0, sharpe = 0.0;
    std::string commit;
    long long runMs = 0;
};

class ResultsDB {
public:
    explicit ResultsDB(const std::string& path = "results/results.db");
    ~ResultsDB();
    void upsert(const RunResult& r);

private:
    void ensureSchema();
    void* db_ = nullptr;  // (sqlite3*)
    std::string path_;
};

} // namespace bt

