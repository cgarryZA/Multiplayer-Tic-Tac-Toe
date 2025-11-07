#include <iostream>
#include <array>

const std::size_t BoardSize = 3;

using Board = std::array<std::array<char, BoardSize>, BoardSize>;

void initBoard(Board &b) 
{
    for (int y = 0; y < BoardSize; ++y)
    {
        for (int x = 0; x < BoardSize; ++x)
        {
            b[y][x] = ' ';
        }
    }
}

void printBoard(const Board &b) 
{
    for (int y = BoardSize - 1; y >= 0; --y) 
    {
        std::cout << " " << y << "   ";
        std::cout << b[y][0] << " | " << b[y][1] << " | " << b[y][2] << "\n";
        if (y > 0) 
        {
            std::cout << "    ---+---+---\n";
        }
    }
    std::cout << "      0   1   2\n";
}

bool placeMove(Board &b, int x, int y, char player) 
{
    if (x < 0 || x > BoardSize - 1 || y < 0 || y > BoardSize - 1) return false;
    if (b[y][x] != ' ') return false;
    b[y][x] = player;
    return true;
}

int main() {
    Board board;
    initBoard(board);

    char current = 'X';

    while (true) 
    {
        printBoard(board);

        std::cout << "Player " << current << " move (x y):";

        int x, y;

        if (!(std::cin >> x >> y)) 
        {
            std::cout << "Input ended.\n";
            break;
        }

        if (!placeMove(board, x, y, current)) 
        {
            std::cout << "Invalid move, try again.\n";
            continue;
        }

        current = (current == 'X') ? 'O' : 'X';
    }

    return 0;
}