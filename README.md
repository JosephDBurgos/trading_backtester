---

```markdown
# Backtrader C++

---

## 🚀 Current Status

✅ **Working prototype complete!**

The framework successfully:
- Loads CSV price data (Open, High, Low, Close, Volume)
- Streams bars through a price feed
- Executes a strategy that makes buy/sell decisions
- Simulates trades with a basic portfolio
- Outputs final equity and trade logs

### Sample Run
```

[Portfolio] Bought 10 shares of AAPL @ 188.63
[MyStrategy] 2025-01-02 00:00:00 | O:187.67 H:188.90 L:186.45 C:188.63 V:54321000
[MyStrategy] 2025-01-03 00:00:00 | O:188.63 H:190.12 L:187.55 C:189.94 V:48732000
[Portfolio] Sold 10 shares of AAPL @ 190.8
[MyStrategy] 2025-01-06 00:00:00 | O:189.10 H:191.00 L:188.20 C:190.80 V:43100000
[Portfolio] Cash: 100022 | Position: 0 | Equity: 100022
Processed 3 bars.

```

---

## ⚙️ Pipeline Overview

The core simulation pipeline looks like this:

```

CSV Data
↓
DataLoader          (loads CSV → vector<PriceBar>)
↓
PriceFeed           (streams bars sequentially)
↓
Backtester::run()   (drives the simulation loop)
↓
Strategy::onBar()   (decides to buy/sell/hold)
↓
Portfolio           (executes trades, updates cash/positions)
↓
Summary             (prints final equity)

```

### High-level Flow

1. **main.cpp** creates a `CsvDataSource`, loads data via `DataLoader`, and feeds it to `PriceFeed`.
2. `Backtester::run()` loops over each `PriceBar` from the feed.
3. Each bar is passed to the active `Strategy`, which calls `Portfolio.buy()` or `Portfolio.sell()` as needed.
4. The `Portfolio` tracks cash, positions, and computes final equity.

---

## 🧩 Project Structure

```

trading_backtester/
├── CMakeLists.txt
├── data/                 # CSV data (e.g., AAPL_1D.csv)
├── include/
│   ├── backtester.h
│   └── schema/
│       ├── csv_data_source.h
│       ├── data_loader.h
│       ├── price_feed.h
│       ├── price_bar.h
│       ├── portfolio.h
│       ├── strategy.h
│       └── ...
├── src/
│   ├── backtester.cpp     # main backtesting loop
│   ├── portfolio.cpp      # portfolio execution logic
│   ├── data_loader.cpp
│   ├── csv_data_source.cpp
│   ├── my_strategy.cpp    # sample strategy
│   └── main.cpp
└── build/

````

---

## 🧠 Core Components

| Component | Responsibility |
|------------|----------------|
| **PriceBar** | Holds OHLCV data for a single bar |
| **CsvDataSource / DataLoader** | Loads and parses CSV data |
| **PriceFeed** | Provides sequential access to bars |
| **Strategy (abstract)** | Base class for all user strategies |
| **MyStrategy** | Example implementation with simple buy/sell logic |
| **Portfolio** | Tracks cash, positions, and equity |
| **Backtester** | Orchestrates the data → strategy → portfolio loop |

---

## 🛠️ Build & Run

### Prerequisites
- C++17 or higher
- CMake ≥ 3.12
- GNU Make or Ninja

### Build Instructions
```bash
mkdir build && cd build
cmake ..
make
./backtrader
````

---

## 🧭 Next Steps

**Planned Enhancements**

* Add a trade log file (`trades.csv`) for analysis
* Compute performance metrics (P&L, total return, win rate, max drawdown)
* Add configurable strategy parameters (e.g., moving average lengths)
* Implement multiple instruments (portfolio per symbol)
* Introduce technical indicators (SMA, EMA, RSI)
* Add CLI or GUI visualization of equity curve and trades

---

## 🧩 Example Strategy Idea (Next Iteration)

```cpp
class MovingAverageStrategy : public Strategy {
    std::deque<double> prices;
    const size_t short_window = 5;
    const size_t long_window = 20;

public:
    void onBar(const PriceBar& bar, Portfolio& portfolio) override {
        prices.push_back(bar.close);
        if (prices.size() < long_window) return;

        double short_avg = average(prices.end() - short_window, prices.end());
        double long_avg  = average(prices.end() - long_window, prices.end());

        if (short_avg > long_avg)
            portfolio.buy("AAPL", bar.close, 10);
        else if (short_avg < long_avg)
            portfolio.sell("AAPL", bar.close, 10);
    }
};
```

---

## 💡 Goal

Learn the fundamentals of modern C++ design by building a practical, extensible trading engine — similar in spirit to Python’s [Backtrader](https://www.backtrader.com/), but developed from scratch in C++.

The ultimate goal is to evolve this project into a robust, multi-asset, configurable backtesting system capable of running and visualizing complex strategies.

```

Absolutely 👍 — here’s your **complete, ready-to-paste `README.md`**, updated to document your current working pipeline, project structure, and next development goals.

---

```markdown
# Backtrader C++

A personal project to learn modern **C++17** by building a modular backtesting framework for trading strategies.

---

## 🚀 Current Status

✅ **Working prototype complete!**

The framework successfully:
- Loads CSV price data (Open, High, Low, Close, Volume)
- Streams bars through a price feed
- Executes a strategy that makes buy/sell decisions
- Simulates trades with a basic portfolio
- Outputs final equity and trade logs

### Sample Run
```

[Portfolio] Bought 10 shares of AAPL @ 188.63
[MyStrategy] 2025-01-02 00:00:00 | O:187.67 H:188.90 L:186.45 C:188.63 V:54321000
[MyStrategy] 2025-01-03 00:00:00 | O:188.63 H:190.12 L:187.55 C:189.94 V:48732000
[Portfolio] Sold 10 shares of AAPL @ 190.8
[MyStrategy] 2025-01-06 00:00:00 | O:189.10 H:191.00 L:188.20 C:190.80 V:43100000
[Portfolio] Cash: 100022 | Position: 0 | Equity: 100022
Processed 3 bars.

```

---

## ⚙️ Pipeline Overview

The core simulation pipeline looks like this:

```

CSV Data
↓
DataLoader          (loads CSV → vector<PriceBar>)
↓
PriceFeed           (streams bars sequentially)
↓
Backtester::run()   (drives the simulation loop)
↓
Strategy::onBar()   (decides to buy/sell/hold)
↓
Portfolio           (executes trades, updates cash/positions)
↓
Summary             (prints final equity)

```

### High-level Flow

1. **main.cpp** creates a `CsvDataSource`, loads data via `DataLoader`, and feeds it to `PriceFeed`.
2. `Backtester::run()` loops over each `PriceBar` from the feed.
3. Each bar is passed to the active `Strategy`, which calls `Portfolio.buy()` or `Portfolio.sell()` as needed.
4. The `Portfolio` tracks cash, positions, and computes final equity.

---

## 🧩 Project Structure

```

trading_backtester/
├── CMakeLists.txt
├── data/                 # CSV data (e.g., AAPL_1D.csv)
├── include/
│   ├── backtester.h
│   └── schema/
│       ├── csv_data_source.h
│       ├── data_loader.h
│       ├── price_feed.h
│       ├── price_bar.h
│       ├── portfolio.h
│       ├── strategy.h
│       └── ...
├── src/
│   ├── backtester.cpp     # main backtesting loop
│   ├── portfolio.cpp      # portfolio execution logic
│   ├── data_loader.cpp
│   ├── csv_data_source.cpp
│   ├── my_strategy.cpp    # sample strategy
│   └── main.cpp
└── build/

````

---

## 🧠 Core Components

| Component | Responsibility |
|------------|----------------|
| **PriceBar** | Holds OHLCV data for a single bar |
| **CsvDataSource / DataLoader** | Loads and parses CSV data |
| **PriceFeed** | Provides sequential access to bars |
| **Strategy (abstract)** | Base class for all user strategies |
| **MyStrategy** | Example implementation with simple buy/sell logic |
| **Portfolio** | Tracks cash, positions, and equity |
| **Backtester** | Orchestrates the data → strategy → portfolio loop |

---

## 🛠️ Build & Run

### Prerequisites
- C++17 or higher
- CMake ≥ 3.12
- GNU Make or Ninja

### Build Instructions
```bash
mkdir build && cd build
cmake ..
make
./backtrader
````


