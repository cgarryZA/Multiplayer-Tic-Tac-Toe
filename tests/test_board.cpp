// tests/test_board.cpp

#include "board.h"
#include "third_party/doctest.h"

TEST_CASE("set() and get() methods work as expected") {
  Board b(3);
  b.set(0, 1, 'X');
  b.set(0, 0, 'O');
  CHECK(b.get(0, 1) == 'X');
  CHECK(b.get(0, 0) == 'O');
}

TEST_CASE("board intitialises with empty cells and correct size") {
  Board b(3);
  CHECK(b.getSize() == 3);
  for (std::size_t y = 0; y < b.getSize(); ++y)
    for (std::size_t x = 0; x < b.getSize(); ++x)
      CHECK(b.get(x, y) == ' ');
}

TEST_CASE("resize() correctly resizes board re-initialises to empty cell") {
  Board b(3);
  CHECK(b.getSize() == 3);
  b.set(0, 1, 'X');
  CHECK(b.get(0, 1) == 'X');
  b.resize(4);
  CHECK(b.getSize() == 4);
  for (std::size_t y = 0; y < b.getSize(); ++y)
    for (std::size_t x = 0; x < b.getSize(); ++x)
      CHECK(b.get(x, y) == ' ');
}

TEST_CASE("isFull() gives False if any cells are empty") {
  Board b(3);
  b.set(0, 1, 'X');
  CHECK(b.isFull() == false);
}

TEST_CASE("isFull() gives True if all cells are occupied") {
  Board b(3);
  for (std::size_t y = 0; y < b.getSize(); ++y)
    for (std::size_t x = 0; x < b.getSize(); ++x)
      b.set(x, y, 'X');
  CHECK(b.isFull() == true);
}
