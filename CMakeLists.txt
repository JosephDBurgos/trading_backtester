cmake_minimum_required(VERSION 3.12)
project(backtrader_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(backtrader
    src/csv_data_source.cpp
    src/data_loader.cpp
    src/my_strategy.cpp
    src/backtester.cpp
    src/trade_logger.cpp
    src/metrics.cpp
    src/portfolio.cpp
    src/main.cpp
)

