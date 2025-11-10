// game.hpp
#pragma once
#include <vector>
#include "board.h"

class Game {
public:
    Game();

    void init(int playerCount);
    bool playTurn();

private:
    char                     checkWinner() const;
    static bool              isBadBoardChar(char ch);
    bool                     placeMove(int x, int y, char player);
    static std::size_t       boardSize(std::size_t numPlayers);
    static std::vector<char> generateRandomSymbols(int count);

    Board             board_;
    std::vector<char> players_;
    std::size_t       currentPlayer_ = 0;
};
