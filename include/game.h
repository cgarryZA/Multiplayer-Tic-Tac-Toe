// game.hpp
#pragma once
#include <vector>
#include "board.h"

class Game {
public:
    Game();

    void init(int playerCount);
    bool playTurn();   // returns false when game ends

private:
    char checkWinner() const;
    bool placeMove(int x, int y, char player);
    static std::size_t boardSize(std::size_t numPlayers);
    static std::vector<char> generateRandomSymbols(int count);
    static bool isBadBoardChar(char ch);

    Board board_;
    std::vector<char> players_;
    std::size_t currentPlayer_ = 0;
};
