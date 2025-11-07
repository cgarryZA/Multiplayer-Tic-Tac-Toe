// main.cpp
#include <iostream>
#include "game.h"

int main()
{
    Game game;

    int players;
    std::cout << "Enter number of players: ";
    if (!(std::cin >> players)) {
        std::cout << "Input ended.\n";
        return 0;
    }

    game.init(players);

    while (game.playTurn()) {
        // loop
    }

    return 0;
}
