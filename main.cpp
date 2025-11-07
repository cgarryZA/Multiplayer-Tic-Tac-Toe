#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

constexpr int MIN_PLAYERS = 2;

struct Board
{
    std::size_t size = 3;
    std::vector<std::vector<char>> cells;
};

struct GameState
{
    Board board;
    std::vector<char> players;
    std::size_t currentPlayer = 0;
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

bool isBadBoardChar(char ch) {
    return ch == '|' || ch == '+' || ch == ' ' || ch == '-';
}

std::vector<char> generateRandomSymbols(int count)
{
    std::vector<char> result;
    result.reserve(count);

    // X and O default first player chars
    if (count >= 1)
        result.push_back('X');
    if (count >= 2)
        result.push_back('O');

    // Pool of random characters
    std::vector<char> pool;
    for (int c = 33; c <= 126; ++c)
    {
        char ch = static_cast<char>(c);
        if (isBadBoardChar(ch))     continue;
        if (ch == 'X' || ch == 'O') continue;
        pool.push_back(ch);
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(pool.begin(), pool.end(), gen);

    for (char ch : pool)
    {
        if ((int)result.size() >= count)
        {
            break;
        }
        result.push_back(ch);
    }

    return result;
}

void initgame(int playerCount, GameState &gs)
{
    gs.players = generateRandomSymbols(playerCount);
    gs.currentPlayer = 0;

    std::size_t size = boardSize(gs.players.size());
    initBoard(gs.board, size);
}

void printBoard(const Board &b)
{
    for (int y = static_cast<int>(b.size) - 1; y >= 0; --y)
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
    if (x < 0 || y < 0)
        return false;
    if (x >= (int)b.size || y >= (int)b.size)
        return false;
    if (b.cells[y][x] != ' ')
        return false;
    b.cells[y][x] = player;
    return true;
}

char checkWinner(const Board &b)
{
    // Check rows
    for (std::size_t y = 0; y < b.size; ++y)
    {
        char first = b.cells[y][0];
        if (first == ' ')
            continue;

        bool allSame = true;

        for (std::size_t x = 1; x < b.size; ++x)
        {
            if (b.cells[y][x] != first)
            {
                allSame = false;
                break;
            }
        }
        if (allSame)
            return first;
    }

    // Check columns
    for (std::size_t x = 0; x < b.size; ++x)
    {
        char first = b.cells[0][x];
        if (first == ' ')
            continue;

        bool allSame = true;

        for (std::size_t y = 1; y < b.size; ++y)
        {
            if (b.cells[y][x] != first)
            {
                allSame = false;
                break;
            }
        }
        if (allSame)
            return first;
    }

    // Check main diagonal
    {
        char first = b.cells[0][0];
        if (first != ' ')
        {
            bool allSame = true;
            for (std::size_t i = 1; i < b.size; ++i)
            {
                if (b.cells[i][i] != first)
                {
                    allSame = false;
                    break;
                }
            }
            if (allSame)
                return first;
        }
    }

    // Check anti-diagonal
    {
        char first = b.cells[0][b.size - 1];
        if (first != ' ')
        {
            bool allSame = true;
            for (std::size_t i = 1; i < b.size; ++i)
            {
                if (b.cells[i][b.size - 1 - i] != first)
                {
                    allSame = false;
                    break;
                }
            }
            if (allSame)
                return first;
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

char getCurrentPlayer()
{
    return game.players[game.currentPlayer];
}

void setNextPlayer()
{
    game.currentPlayer = (game.currentPlayer + 1) % game.players.size();
}

bool playTurn(GameState &game)
{
    printBoard(game.board);

    char currentPlayer = getCurrentPlayer();
    
    std::cout << "Player " << currentPlayer << " move (x y): ";

    int x, y;
    if (!(std::cin >> x >> y))
    {
        std::cout << "Input ended.\n";
        return false;
    }

    if (!placeMove(game.board, x, y, currentPlayer))
    {
        std::cout << "Invalid move, try again.\n";
        return true;
    }

    char winner = checkWinner(game.board);
    if (winner != ' ')
    {
        printBoard(game.board);
        std::cout << "Player " << winner << " wins!\n";
        return false;
    }
    if (checkStaleMate(game.board))
    {
        printBoard(game.board);
        std::cout << "It's a draw.\n";
        return false;
    }

    setNextPlayer();
    return true;
}

int main()
{
    GameState game;

    int playerCount;

    std::cout << "Enter number of players: ";
    if (!(std::cin >> playerCount))
    {
        std::cout << "Input ended.\n";
        return 0;
    }

    if (playerCount < MIN_PLAYERS)
    {
        playerCount = MIN_PLAYERS;
    }

    initgame(playerCount, game);

    while (playTurn(game))
    {
        //Main Loop
    }

    return 0;
}