cmake_minimum_required(VERSION 3.15)

project(MultiplayerTicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# shared logic (board + local game)
add_library(tictactoe
    src/board.cpp
    src/game.cpp
)

target_include_directories(tictactoe
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# local executable
add_executable(mttt
    src/main.cpp
)
target_link_libraries(mttt PRIVATE tictactoe)

# server (network)
add_executable(ttt_server
    src/server.cpp
)
target_link_libraries(ttt_server PRIVATE tictactoe ws2_32)

# client (network)
add_executable(ttt_client
    src/client.cpp
)
target_link_libraries(ttt_client PRIVATE tictactoe ws2_32)
