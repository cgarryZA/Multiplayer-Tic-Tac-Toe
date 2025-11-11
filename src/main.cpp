// main.cpp
#include <iostream>
#include "game.h"

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
        // loop
    }

    return 0;
}

int main()
{
    try {
        return run_app(std::cin, std::cout);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}
