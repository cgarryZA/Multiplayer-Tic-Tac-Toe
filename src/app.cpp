// src/app.cpp

#include "app.h"
#include "game.h"
#include "codes.h"
#include <string>

int run_app(std::istream& in,
            std::ostream& out,
            std::function<void()> game_loop)
{
    Game game;

    out << "Enter number of players: ";
    int players;
    if (!(in >> players)) {
        out << "Input ended.\n";
        return APP_INPUT_EOF;
    }
    if (players <= 0) {
        out << "Invalid number of players.\n";
        return APP_INPUT_INVALID;
    }

    game.init(players);

    if (game_loop) {
        game_loop();
    } else {
        while (game.playTurn()) {
        }
    }

    return APP_OK;
}
