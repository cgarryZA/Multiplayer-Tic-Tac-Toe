// board.cpp
#include "board.h"

Board::Board(std::size_t size) : size_(size), cells_(size, std::vector<char>(size, ' '))
{
}

void Board::resize(std::size_t newSize)
{
    size_ = newSize;
    cells_.assign(size_, std::vector<char>(size_, ' '));
}

char Board::get(std::size_t x, std::size_t y) const
{
    return cells_[y][x];
}

void Board::set(std::size_t x, std::size_t y, char value)
{
    cells_[y][x] = value;
}

void Board::print() const
{
    for (int y = static_cast<int>(size_) - 1; y >= 0; --y)
    {
        std::cout << " " << y << "   ";
        for (std::size_t x = 0; x < size_; ++x)
        {
            std::cout << get(x, y);
            if (x < size_ - 1)
                std::cout << " | ";
        }
        std::cout << "\n";

        if (y > 0)
        {
            std::cout << "    ";
            for (std::size_t x = 0; x < size_; ++x)
            {
                std::cout << "---";
                if (x < size_ - 1)
                    std::cout << "+";
            }
            std::cout << "\n";
        }
    }

    std::cout << "      ";
    for (std::size_t x = 0; x < size_; ++x)
        std::cout << x << "   ";
    std::cout << "\n";
}

bool Board::isFull() const
{
    for (std::size_t y = 0; y < size_; ++y)
        for (std::size_t x = 0; x < size_; ++x)
            if (get(x, y) == ' ')
                return false;
    return true;
}
