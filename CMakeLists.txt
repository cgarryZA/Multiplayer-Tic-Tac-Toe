cmake_minimum_required(VERSION 3.15)

project(MultiplayerTicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

# ---- coverage ----
option(ENABLE_COVERAGE "Enable coverage flags" OFF)
if (ENABLE_COVERAGE)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif()
endif()

# library with the game logic
add_library(tictactoe
    src/board.cpp
    src/game.cpp
)

target_include_directories(tictactoe
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# final executable
add_executable(mttt
    src/main.cpp
)
target_link_libraries(mttt PRIVATE tictactoe)

# ---- Unit tests (doctest) ----
add_executable(unit_tests
    tests/test_game.cpp
    tests/test_main.cpp
)
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(unit_tests PRIVATE tictactoe)

add_test(NAME unit COMMAND unit_tests)
