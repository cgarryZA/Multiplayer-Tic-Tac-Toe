// include/game.h

#pragma once
#include <vector>
#include "board.h"

class Game {
public:
    Game();

    void init(int playerCount);
    static std::size_t boardSize(std::size_t numPlayers);
    bool               placeMove(int x, int y, char player);
    char               checkWinner() const;
    bool               playTurn();

private:
    static bool              isBadBoardChar(char ch);
    static std::vector<char> generateRandomSymbols(int count);

    Board             board_;
    std::vector<char> players_;
    std::size_t       currentPlayer_ = 0;
};
