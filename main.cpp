#include <iostream>
#include <array>
#include <vector>

constexpr std::array<char, 5> SYMBOLS{'X', 'Y', 'Z', 'A', 'B'};

struct Board
{
    std::size_t                    size = 3;
    std::vector<std::vector<char>> cells;
};

struct GameState
{
    Board             board;
    std::vector<char> players;
    std::size_t       currentPlayer = 0;
    
};

std::size_t boardSize(std::size_t numPlayers)
{
    return numPlayers + 1;
}

char nextSymbol(int playerIndex)
{
    return SYMBOLS[playerIndex];
}

void initBoard(Board &b, std::size_t newSize) 
{
    b.size = newSize;
    b.cells.assign(b.size, std::vector<char>(b.size, ' '));
}

void initgame(int playerCount, GameState &gs)
{
    gs.players.clear();

    if (playerCount > (int)SYMBOLS.size())
    {
        playerCount = (int)SYMBOLS.size();
    }

    for (int i = 0; i < playerCount; ++i)
    {
        gs.players.push_back(nextSymbol(i));
    }
    
    gs.currentPlayer = 0;

    std::size_t size = boardSize(gs.players.size());

    initBoard(gs.board, size);
}

void printBoard(const Board &b) 
{
    for (int y = (int)b.size - 1; y >= 0; --y) 
    {
        std::cout << " " << y << "   ";
        for (std::size_t x = 0; x < b.size; ++x)
        {
            std::cout << b.cells[y][x];
            if (x < b.size - 1)
            {
                std::cout << " | ";
            }
        }
        std::cout << "\n";

        if (y > 0) 
        {
            std::cout << "    ";
            for (std::size_t x = 0; x < b.size; ++x)
            {
                std::cout << "---";
                if (x < b.size - 1)
                {
                    std::cout << "+";
                }
            }
            std::cout << "\n";
        }
    }

    std::cout << "      ";
    for (std::size_t x = 0; x < b.size; ++x)
    {
        std::cout << x << "   ";
    }
    std::cout << "\n";
}

bool placeMove(Board &b, int x, int y, char player) 
{
    if (x < 0 || y < 0) return false;
    if (x >= (int)b.size || y >= (int)b.size) return false;
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

bool checkStaleMate(const Board &b)
{
    for (std::size_t y = 0; y < b.size; ++y)
        for (std::size_t x = 0; x < b.size; ++x)
            if (b.cells[y][x] == ' ')
                return false;
    return true;
}

std::string checkGameState(const Board &b)
{
    char winner = checkWinner(b);
    if (winner != ' ') 
    {
        printBoard(b);
        return std::string("Player ") + std::string(1, winner) + " wins!\n";
    }
    else if (winner == '+')
    {
        printBoard(b);
        return "It's a draw.\n";
    }

    return " ";
}

int main() {
    GameState game;

    int playerCount;

    std::cout << "Enter number of players: ";
    if (!(std::cin >> playerCount)) 
        {
            std::cout << "Input ended.\n";
            return 0;
        }
    
    if (playerCount < 2)
    {
        playerCount = 2;
    }
    if (playerCount > (int)SYMBOLS.size())
    {
        playerCount = (int)SYMBOLS.size();
    }

    initgame(playerCount, game);

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
        if (checkStaleMate(game.board))
        {
            printBoard(game.board);
            std::cout << "It's a draw.\n";
            break;
        }
        
        game.currentPlayer = (game.currentPlayer + 1) % game.players.size();
    }

    return 0;
}