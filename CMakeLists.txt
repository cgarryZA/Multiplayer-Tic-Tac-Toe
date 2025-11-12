cmake_minimum_required(VERSION 3.15)

project(MultiplayerTicTacToe LANGUAGES CXX)

# use C++17 (or 20 if you want)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

# library with the game logic
add_library(tictactoe
    src/board.cpp
    src/game.cpp
)

# let that library see the headers in include/
target_include_directories(tictactoe
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# final executable
add_executable(mttt
    src/main.cpp
)

# link the game logic into the executable
target_link_libraries(mttt
    PRIVATE
        tictactoe
)

# ---- Unit tests (doctest) ----
add_executable(unit_tests
    tests/test_game.cpp
)
target_include_directories(unit_tests PRIVATE 
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_link_libraries(unit_tests PRIVATE tictactoe)

add_test(NAME unit COMMAND unit_tests)

