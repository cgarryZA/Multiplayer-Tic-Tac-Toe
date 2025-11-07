cmake_minimum_required(VERSION 3.15)

project(MultiplayerTicTacToe LANGUAGES CXX)

# use C++17 (or 20 if you want)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
