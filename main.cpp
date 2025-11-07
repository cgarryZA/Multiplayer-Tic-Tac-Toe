#include <iostream>
#include <array>
#include <vector>

struct Board
{
    std::size_t size = 3;
    std::vector<std::vector<char>> cells;
};

struct GameState
{
    std::vector<char> players;
    std::size_t currentPlayer = 0;

    Board board;
};

std::size_t boardSize(std::size_t numPlayers)
{
    return numPlayers + 1;
}

void initBoard(Board &b, std::size_t newSize) 
{
    b.size = newSize;
    b.cells.assign(b.size, std::vector<char>(b.size, ' '));
}

void initgame(GameState &gs)
{
    gs.players.clear();
    gs.players.push_back('X');
    gs.players.push_back('O');
    gs.currentPlayer = 0;

    std::size_t size = boardSize(gs.players.size());

    initBoard(gs.board, size);
}

void printBoard(const Board &b) 
{
    for (int y = b.size - 1; y >= 0; --y) 
    {
        std::cout << " " << y << "   ";
        std::cout << b.cells[y][0] << " | " << b.cells[y][1] << " | " << b.cells[y][2] << "\n";
        if (y > 0) 
        {
            std::cout << "    ---+---+---\n";
        }
    }
    std::cout << "      0   1   2\n";
}

bool placeMove(Board &b, int x, int y, char player) 
{
    if (x < 0 || x > b.size - 1 || y < 0 || y > b.size - 1) return false;
    if (b.cells[y][x] != ' ') return false;
    b.cells[y][x] = player;
    return true;
}

char checkWinner(const Board &b)
{
    //Check rows
    for (int y = 0; y < b.size; ++y)
    {
        char first = b.cells[y][0];
        if (first == ' ') continue;

        bool allSame = true;

        for (int x = 1; x < b.size; ++x)
        {
            if (b.cells[y][x] != first)
            {
                allSame = false;
                break;
            }
        }
        if (allSame) return first;
    }

    //Check columns
    for (int x = 0; x < b.size; ++x)
    {
        char first = b.cells[0][x];
        if (first == ' ') continue;

        bool allSame = true;

        for (int y = 1; y < b.size; ++y)
        {
            if (b.cells[y][x] != first)
            {
                allSame = false;
                break;
            }
        }
        if (allSame) return first;
    }

    //Check main diagonal
    {
        char first = b.cells[0][0];
        if (first != ' ')
        {
            bool allSame = true;
            for (int i = 1; i < b.size; ++i)
            {
                if (b.cells[i][i] != first)
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
        char first = b.cells[0][b.size - 1];
        if (first != ' ')
        {
            bool allSame = true;
            for (int i = 1; i < b.size; ++i)
            {
                if (b.cells[i][b.size - 1 - i] != first)
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
    for (int y = 0; y < b.size; ++y)
    {
        for (int x = 0; x < b.size; ++x)
        {
            if (b.cells[y][x] == ' ')
            {
                return false;
            }
        }
    }
    return true;
}

int main() {
    GameState game;

    initgame(game);

    while (true) 
    {
        printBoard(game.board);

        char currentPlayer = game.players[game.currentPlayer];
        std::cout << "Player " << currentPlayer << " move (x y): ";

        int x, y;

        if (!(std::cin >> x >> y)) 
        {
            std::cout << "Input ended.\n";
            break;
        }

        if (!placeMove(game.board, x, y, currentPlayer)) 
        {
            std::cout << "Invalid move, try again.\n";
            continue;
        }

        char winner = checkWinner(game.board);
        if (winner != ' ') 
        {
            printBoard(game.board);
            std::cout << "Player " << winner << " wins!\n";
            break;
        }
        
        if (checkStalemate(game.board))
        {
            printBoard(game.board);
            std::cout << "It's a draw.\n";
            break;
        }

        game.currentPlayer = (game.currentPlayer + 1) % game.players.size();
    }

    return 0;
}