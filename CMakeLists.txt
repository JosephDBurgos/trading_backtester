find_package(SQLite3 REQUIRED)
file(GLOB SOURCES "src/*.cpp")
add_executable(backtrader ${SOURCES})
target_include_directories(backtrader PRIVATE include ${SQLite3_INCLUDE_DIRS})
target_link_libraries(backtrader PRIVATE ${SQLite3_LIBRARIES})
set(CMAKE_CXX_STANDARD 17)

