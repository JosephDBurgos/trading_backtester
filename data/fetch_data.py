#!/usr/bin/env python3
import yfinance as yf
import pandas as pd
import os
from datetime import datetime, timedelta

# === CONFIGURATION ===
symbols = ["AAPL", "MSFT", "NVDA", "GOOG", "AMZN", "META", "TSLA"]
end_date = datetime.today()
start_date = end_date - timedelta(days=180)
interval = "1d"
# ======================

start_date = start_date.strftime("%Y-%m-%d")
end_date = end_date.strftime("%Y-%m-%d")


for symbol in symbols:
    print(
        f"[Data Fetcher] Downloading {symbol} data from {start_date} to {end_date}..."
    )

    df = yf.download(
        symbol, start=start_date, end=end_date, interval=interval, group_by="ticker"
    )

    if df.empty:
        print(f"[Error] No data downloaded for {symbol}.")
        continue

    # ✅ Flatten MultiIndex if necessary
    if isinstance(df.columns, pd.MultiIndex):
        df.columns = [" ".join(col).strip() for col in df.columns.values]

    # ✅ Try to rename any prefixed columns like "AAPL Open" → "Open"
    rename_map = {}
    for col in df.columns:
        parts = col.split()
        if len(parts) == 2 and parts[1] in ["Open", "High", "Low", "Close", "Volume"]:
            rename_map[col] = parts[1]

    df.rename(columns=rename_map, inplace=True)

    # ✅ Verify expected columns exist
    missing_cols = [
        c for c in ["Open", "High", "Low", "Close", "Volume"] if c not in df.columns
    ]
    if missing_cols:
        print(
            f"[Warning] Missing columns {missing_cols}, available: {list(df.columns)}"
        )
        continue

    # ✅ Keep only the expected columns
    df = df[["Open", "High", "Low", "Close", "Volume"]]

    # ✅ Reset index to make Date a normal column
    df.reset_index(inplace=True)

    # ✅ Save clean CSV (compatible with your C++ parser)
    filename = f"{symbol}_1D.csv"
    df.to_csv(
        filename,
        index=False,
        columns=["Date", "Open", "High", "Low", "Close", "Volume"],
    )

    print(f"[Data Fetcher] Saved clean data → {filename} ({len(df)} rows)")
