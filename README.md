# Multiplayer-Tic-Tac-Toe

[![CI](https://github.com/cgarryZA/Multiplayer-Tic-Tac-Toe/actions/workflows/ci.yml/badge.svg)](https://github.com/cgarryZA/Multiplayer-Tic-Tac-Toe/actions/workflows/ci.yml)
[![Coverage](https://codecov.io/gh/cgarryZA/Multiplayer-Tic-Tac-Toe/branch/main/graph/badge.svg)](https://codecov.io/gh/cgarryZA/Multiplayer-Tic-Tac-Toe)

Multiplayer tic tac toe game with build tools and CI implementations

## Contributors ##
1. Louis Bedford
2. Christian Garry
3. Ailis McConachie-Smith
4. Matvii Prytula

## Game Overview ##
This is a multiplayer tic-tac-toe game played with N players on an (N+1) X (N+1) sized board. 
Each player is represented by their own symbol (e.g. 'X', 'O' etc)
Each player takes a turn placing a symbol on the board until the board is full or the game is won (rules are described below)

## Game Format ##
```text
 2   X | O | 
    ---+---+---
 1   O | X |  
    ---+---+---
 0     |   |  
     0   1   2
```
move looks like (x, y)

## Rules ##
- The game is initialised with N players and a symbol is generated for each player
- The board is intialized as a square (N+1) X (N+1) grid
- Players take it in turns to place their symbol in a square on the grid - a move has the form (x, y)
- A WIN is achieved when a players fills a row, column, or diagonal with their symbol
- If the board is filled before a win is achieved then the result is a DRAW


## Classes ##
board:
This class stores the board as a 2D grid, providing the size of the board, the `get()` and `set()` methods, and the `isFull()` method for checking.

game:
This class manages the board and the players. It handles the moves and checks for winning players or if the board is full.

## Dependencies ##
- CMake 3.15 or higher
- C++17 compiler
- Git (to clone the repository)

## General Installation Instructions ##
- **Clone the project and create a build directory inside the project folder**
  ```bash
  git clone <link>
  cd Multiplayer-Tic-Tac-toe
  mkdir build
  cd build
  ```
- **Use CMake to compile the project files**
  ```bash
  cmake ..
  cmake --build .
  ```
- **Run the executable from the terminal**
  ```bash
  ./mttt
  ```
- Successful installation will be prompted with the command `'Enter number of players:'`

## Windows Installation ##
- **Clone the project and create a build directory inside the project folder**
  ```bash
  git clone <link>
  cd Multiplayer-Tic-Tac-toe
  cd scripts
  ```
- **Debug version with tests
  ```bash
  .\build.bat
  .\coverage.bat  #generate coverage html for tests
  ```
- **Release version
  ```bash
  .\build_release.bat
  ```
- **Run
  ```bash
  .\run.bat
  ```
- Successful installation will be prompted with the command `'Enter number of players:'`

## Testing ##
This project uses the `doctest` framework with test files located in the `tests/` directory

**CMake handles testing** — just run:
 ```bash
 cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
 cmake --build build
 ctest --test-dir build --output-on-failure
  ```

## LICENSE ##
This project is released under the MIT License. See the LICENSE file for details.

