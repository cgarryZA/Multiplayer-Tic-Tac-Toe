// tests/test_board.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "third_party/doctest.h"
#include "board.h"

TEST_CASE("board intitialises with empty cells and correct size") {
Board b(3);
CHECK(b.getSize() == 3);
for (std::size_t y = 0; y < b.getSize(); ++y)
  for (std::size_t x = 0; x < b.getSize(); ++x)
    CHECK(b.get(x, y) == ' ');
}
