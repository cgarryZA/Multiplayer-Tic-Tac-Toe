// board.hpp
#pragma once
#include <cstddef>
#include <iostream>
#include <vector>

class Board {
public:
  explicit Board(std::size_t size = 3);

  void resize(std::size_t newSize);
  std::size_t getSize() const { return size_; }

  char get(std::size_t x, std::size_t y) const;
  void set(std::size_t x, std::size_t y, char value);

  void print() const;
  bool isFull() const;

private:
  std::size_t size_;
  std::vector<std::vector<char>> cells_;
};
