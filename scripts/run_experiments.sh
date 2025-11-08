#!/bin/bash
mkdir -p results
for sym in AAPL MSFT TSLA; do
  ./backtrader --strategy my_strategy --symbol $sym --timeframe 1D
done

