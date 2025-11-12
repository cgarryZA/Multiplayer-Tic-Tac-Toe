// main.cpp
#include "game.h"
#include <iostream>

int main() {
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
