// src/app.cpp
#include "app.h"
#include "game.h"
#include <iostream>

int run_app(std::istream& in, std::ostream& out)
{
    Game game;

    out << "Enter number of players: ";
    int players;
    if (!(in >> players)) {
        out << "Input ended.\n";
        return 0;
    }

    game.init(players);

    while (game.playTurn()) {
        // play
    }

    return 0;
}
