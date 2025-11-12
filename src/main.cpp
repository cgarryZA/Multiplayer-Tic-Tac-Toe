// src/main.cpp

#include <iostream>
#include "app.h"
#include "game.h"

int main()
{
    Game game;

    auto init = [&game](int players) -> bool {
        game.init(players);
        return true;
    };

    auto turn = [&game]() -> bool {
        return game.playTurn();
    };

    return run_app(std::cin, std::cout, init, turn);
}
