cmake_minimum_required(VERSION 3.20)
project(StockAnalysis C)

set(CMAKE_C_STANDARD 11)

# ── Interactive binary ──────────────────────────────────────
add_executable(stock_analysis stock_analysis.c)
target_link_libraries(stock_analysis m)

# ── Test binary ─────────────────────────────────────────────
add_executable(stock_tests
        stock_analysis.c
        LibCUnit/Automated.c
        LibCUnit/CUError.c
        LibCUnit/TestDB.c
        LibCUnit/TestRun.c
        LibCUnit/Util.c
        LibCUnit/Basic.c
        LibCUnit/MyMem.c
        LibCUnit/Console.c
)
target_compile_definitions(stock_tests PRIVATE RUN_TESTS)
target_include_directories(stock_tests PRIVATE LibCUnit)
target_link_libraries(stock_tests m)
