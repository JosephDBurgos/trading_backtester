#include "schema/results_db.h"
#include <sqlite3.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
namespace bt {

ResultsDB::ResultsDB(const std::string& path) : path_(path) {
    fs::create_directories(fs::path(path_).parent_path());
    if (sqlite3_open(path_.c_str(), reinterpret_cast<sqlite3**>(&db_)) != SQLITE_OK) {
        throw std::runtime_error("Cannot open SQLite DB: " + path_);
    }
    ensureSchema();
}

ResultsDB::~ResultsDB() {
    if (db_) sqlite3_close(static_cast<sqlite3*>(db_));
}

void ResultsDB::ensureSchema() {
    const char* sql = R"(
    CREATE TABLE IF NOT EXISTS results (
        strategy   TEXT,
        symbol     TEXT,
        timeframe  TEXT,
        start      TEXT,
        end        TEXT,
        trades     INTEGER,
        win_rate   REAL,
        total_pnl  REAL,
        avg_pnl    REAL,
        max_dd     REAL,
        sharpe     REAL,
        git_commit     TEXT,
        run_ms     INTEGER,
        PRIMARY KEY (strategy, symbol, timeframe, start, end)
    );
    )";
    char* err = nullptr;
    if (sqlite3_exec(static_cast<sqlite3*>(db_), sql, nullptr, nullptr, &err) != SQLITE_OK) {
        std::string e = err ? err : "";
        sqlite3_free(err);
        throw std::runtime_error("SQLite schema error: " + e);
    }
}

void ResultsDB::upsert(const RunResult& r) {
    const char* sql = R"(
        INSERT INTO results
        (strategy, symbol, timeframe, start, end, trades, win_rate, total_pnl,
         avg_pnl, max_dd, sharpe, git_commit, run_ms)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        ON CONFLICT(strategy, symbol, timeframe, start, end)
        DO UPDATE SET
            trades=excluded.trades,
            win_rate=excluded.win_rate,
            total_pnl=excluded.total_pnl,
            avg_pnl=excluded.avg_pnl,
            max_dd=excluded.max_dd,
            sharpe=excluded.sharpe,
            git_commit=excluded.git_commit,
            run_ms=excluded.run_ms;
    )";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(static_cast<sqlite3*>(db_), sql, -1, &stmt, nullptr);
    int i = 1;
    sqlite3_bind_text (stmt, i++, r.strategy.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text (stmt, i++, r.symbol.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text (stmt, i++, r.timeframe.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text (stmt, i++, r.start.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text (stmt, i++, r.end.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int  (stmt, i++, r.trades);
    sqlite3_bind_double(stmt, i++, r.winRate);
    sqlite3_bind_double(stmt, i++, r.totalPnL);
    sqlite3_bind_double(stmt, i++, r.avgPnL);
    sqlite3_bind_double(stmt, i++, r.maxDD);
    sqlite3_bind_double(stmt, i++, r.sharpe);
    sqlite3_bind_text (stmt, i++, r.commit.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, i++, r.runMs);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "[ResultsDB] SQLite insert error: "
                  << sqlite3_errmsg(static_cast<sqlite3*>(db_)) << "\n";
    }

    sqlite3_finalize(stmt);
}

} // namespace bt

