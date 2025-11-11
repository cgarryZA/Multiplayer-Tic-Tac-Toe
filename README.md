# Multiplayer-Tic-Tac-Toe
Multiplayer tic tac toe game with build tools and CI implementations

Format
```text
 2   X | O | 
    ---+---+---
 1   O | X |  
    ---+---+---
 0     |   |  
     0   1   2

move looks like (x, y)
```
### Notes for the team (tests + CI)

- **Tests use `doctest`** (header-only, no install needed).  
  All test files are in `tests/`, and the main one right now is `tests/test_game.cpp`.  
  The header lives at `tests/third_party/doctest.h`.

- **CMake handles testing** — just run:
  ```bash
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
  cmake --build build
  ctest --test-dir build --output-on-failure
    ```
- **Adding tests**:
Just add new TEST_CASEs in `tests/test_game.cpp` or a new `.cpp` under `tests/`.