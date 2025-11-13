cmake_minimum_required(VERSION 3.15)

project(MultiplayerTicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

option(BUILD_TESTS "Build unit tests" ON)
option(ENABLE_COVERAGE "Enable coverage flags" OFF)

if (ENABLE_COVERAGE)
    add_compile_options(-O0 -g)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
        add_link_options(-fprofile-instr-generate -fcoverage-mapping)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif()
endif()

add_library(tictactoe
    src/board.cpp
    src/game.cpp
    src/app.cpp
)

target_include_directories(tictactoe
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(mttt
    src/main.cpp
)
target_link_libraries(mttt PRIVATE tictactoe)

if (BUILD_TESTS)
    add_executable(unit_tests
        tests/test_game.cpp
        tests/test_app.cpp
        tests/test_app_header.cpp
        tests/test_teams.cpp
    )
    target_include_directories(unit_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
    target_link_libraries(unit_tests PRIVATE tictactoe)

    add_test(NAME unit_all COMMAND unit_tests)
    add_test(NAME unit_app_only COMMAND unit_tests --test-case "app:*")
endif()

# Files to run clang-format on
set(ALL_CODE
    src/app.cpp
    src/board.cpp
    src/game.cpp
    src/main.cpp

    include/app.h
    include/board.h
    include/codes.h
    include/game.h

    tests/test_app.cpp
    tests/test_app_header.cpp
    tests/test_game.cpp
    tests/test_teams.cpp
)

find_program(CLANG_FORMAT_EXE NAMES clang-format)
if (CLANG_FORMAT_EXE)
  add_custom_target(format-check
    COMMAND ${CLANG_FORMAT_EXE} -n -Werror --style=file ${ALL_CODE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format check"
  )
  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i --style=file ${ALL_CODE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Applying clang-format"
  )
endif()
