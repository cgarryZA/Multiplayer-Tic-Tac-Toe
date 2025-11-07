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

char checkWinner(const Board &b)
{
    //Check rows
    for (int y = 0; y < (int)BoardSize; ++y)
    {
        char first = b[y][0];
        if (first == ' ') continue;

        bool allSame = true;

        for (int x = 1; x < (int)BoardSize; ++x)
        {
            if (b[y][x] != first)
            {
                allSame = false;
                break;
            }
        }
        if (allSame) return first;
    }

    //Check columns
    for (int x = 0; x < (int)BoardSize; ++x)
    {
        char first = b[0][x];
        if (first == ' ') continue;

        bool allSame = true;

        for (int y = 1; y < (int)BoardSize; ++y)
        {
            if (b[y][x] != first)
            {
                allSame = false;
                break;
            }
        }
        if (allSame) return first;
    }

    //Check main diagonal
    {
        char first = b[0][0];
        if (first != ' ')
        {
            bool allSame = true;
            for (int i = 1; i < (int)BoardSize; ++i)
            {
                if (b[i][i] != first)
                {
                    allSame = false;
                    break;
                }
            }
            if (allSame) return first;
        }
    }

    //Check anti-diagonal
    {
        char first = b[0][BoardSize - 1];
        if (first != ' ')
        {
            bool allSame = true;
            for (int i = 1; i < (int)BoardSize; ++i)
            {
                if (b[i][BoardSize - 1 - i] != first)
                {
                    allSame = false;
                    break;
                }
            }
            if (allSame) return first;
        }
    }
    return ' ';
}

bool checkStalemate(const Board &b) 
{
    for (int y = 0; y < (int)BoardSize; ++y)
    {
        for (int x = 0; x < (int)BoardSize; ++x)
        {
            if (b[y][x] == ' ')
            {
                return false;
            }
        }
    }
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

        char winner = checkWinner(board);
        if (winner != ' ') 
        {
            printBoard(board);
            std::cout << "Player " << winner << " wins!\n";
            break;
        }
        
        if (checkStalemate(board))
        {
            printBoard(board);
            std::cout << "It's a draw.\n";
            break;
        }

        current = (current == 'X') ? 'O' : 'X';
    }

    return 0;
}