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

# ----------------------
# add googletest
# ----------------------
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# Actually download and make the library available
FetchContent_MakeAvailable(googletest)

enable_testing()

# test executable
add_executable(
  hello_test
  tests/hello_test.cc   # make sure your test file is here
)

target_link_libraries(
  hello_test
  PRIVATE
    tictactoe            # link your library if needed
    gtest_main           # GoogleTest main function
)

include(GoogleTest)
gtest_discover_tests(hello_test)

