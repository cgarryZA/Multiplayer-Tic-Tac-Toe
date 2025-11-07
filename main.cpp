#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

constexpr int MIN_PLAYERS = 2;

struct Board
{
    std::size_t size = 3;
    std::vector<std::vector<char>> cells;

    char get(std::size_t x, std::size_t y) const {
        return cells[y][x];
    }

    void set(std::size_t x, std::size_t y, char value) {
        cells[y][x] = value;
    }

    void print() const {
        for (int y = static_cast<int>(size) - 1; y >= 0; --y)
        {
            std::cout << " " << y << "   ";
            for (std::size_t x = 0; x < size; ++x)
            {
                std::cout << get(x, y);
                if (x < size - 1)
                    std::cout << " | ";
            }
            std::cout << "\n";

            if (y > 0)
            {
                std::cout << "    ";
                for (std::size_t x = 0; x < size; ++x)
                {
                    std::cout << "---";
                    if (x < size - 1)
                        std::cout << "+";
                }
                std::cout << "\n";
            }
        }

        std::cout << "      ";
        for (std::size_t x = 0; x < size; ++x)
            std::cout << x << "   ";
        std::cout << "\n";
    }

    bool checkStaleMate() const {
        for (std::size_t y = 0; y < size; ++y)
            for (std::size_t x = 0; x < size; ++x)
                if (get(x, y) == ' ')
                    return false;
        return true;
    }
};

std::size_t boardSize(std::size_t numPlayers) {
    return numPlayers + 1;
}

bool isBadBoardChar(char ch) {
    return ch == '|' || ch == '+' || ch == ' ' || ch == '-';
}

std::vector<char> generateRandomSymbols(int count)
{
    std::vector<char> result;
    result.reserve(count);

    if (count >= 1) result.push_back('X');
    if (count >= 2) result.push_back('O');
    if ((int)result.size() >= count)
        return result;

    std::vector<char> pool;
    for (int c = 33; c <= 126; ++c)
    {
        char ch = static_cast<char>(c);
        if (isBadBoardChar(ch)) continue;
        if (ch == 'X' || ch == 'O') continue;
        pool.push_back(ch);
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(pool.begin(), pool.end(), gen);

    for (char ch : pool)
    {
        if ((int)result.size() >= count)
            break;
        result.push_back(ch);
    }

    return result;
}

void initBoard(Board &b, std::size_t newSize)
{
    b.size = newSize;
    b.cells.assign(b.size, std::vector<char>(b.size, ' '));
}

struct GameState
{
    Board board;
    std::vector<char> players;
    std::size_t currentPlayer = 0;

    void setNextPlayer() {
        currentPlayer = (currentPlayer + 1) % players.size();
    }

    char getCurrentPlayer() const {
        return players[currentPlayer];
    }

    void initgame(int playerCount) {
        players = generateRandomSymbols(playerCount);
        currentPlayer = 0;
        initBoard(board, boardSize(players.size()));
    }
};

bool placeMove(Board &b, int x, int y, char player)
{
    if (x < 0 || y < 0) return false;
    if (x >= (int)b.size || y >= (int)b.size) return false;
    if (b.get(x, y) != ' ') return false;
    b.set(x, y, player);
    return true;
}

char checkWinner(const Board &b)
{
    // rows
    for (std::size_t y = 0; y < b.size; ++y)
    {
        char first = b.get(0, y);
        if (first == ' ') continue;

        bool allSame = true;
        for (std::size_t x = 1; x < b.size; ++x)
        {
            if (b.get(x, y) != first) {
                allSame = false;
                break;
            }
        }
        if (allSame) return first;
    }

    // cols
    for (std::size_t x = 0; x < b.size; ++x)
    {
        char first = b.get(x, 0);
        if (first == ' ') continue;

        bool allSame = true;
        for (std::size_t y = 1; y < b.size; ++y)
        {
            if (b.get(x, y) != first) {
                allSame = false;
                break;
            }
        }
        if (allSame) return first;
    }

    // main diag
    {
        char first = b.get(0, 0);
        if (first != ' ')
        {
            bool allSame = true;
            for (std::size_t i = 1; i < b.size; ++i)
            {
                if (b.get(i, i) != first) {
                    allSame = false;
                    break;
                }
            }
            if (allSame) return first;
        }
    }

    // anti diag
    {
        char first = b.get(b.size - 1, 0);
        if (first != ' ')
        {
            bool allSame = true;
            for (std::size_t i = 1; i < b.size; ++i)
            {
                if (b.get(b.size - 1 - i, i) != first) {
                    allSame = false;
                    break;
                }
            }
            if (allSame) return first;
        }
    }

    return ' ';
}

bool playTurn(GameState &game)
{
    game.board.print();

    char currentPlayer = game.getCurrentPlayer();
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
        game.board.print();
        std::cout << "Player " << winner << " wins!\n";
        return false;
    }

    if (game.board.checkStaleMate())
    {
        game.board.print();
        std::cout << "It's a draw.\n";
        return false;
    }

    game.setNextPlayer();
    return true;
}

int getPlayerCount()
{
    int playerCount;
    std::cout << "Enter number of players: ";
    if (!(std::cin >> playerCount))
    {
        std::cout << "Input ended.\n";
        return MIN_PLAYERS;
    }

    if (playerCount < MIN_PLAYERS)
        playerCount = MIN_PLAYERS;

    return playerCount;
}

int main()
{
    GameState game;

    int playerCount = getPlayerCount();
    game.initgame(playerCount);

    while (playTurn(game)) {
        // main loop
    }

    return 0;
}
