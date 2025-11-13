// src/app.cpp

#include "app.h"
#include "codes.h"

#include <functional>
#include <iostream>

int run_app(std::istream &in, std::ostream &out,
            const std::function<bool(int)> &init_fn,
            const std::function<bool()> &turn_fn) {
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

  if (!init_fn(players)) {
    out << "Failed to init.\n";
    return APP_INPUT_INVALID;
  }

  while (turn_fn()) {
    // keep playing turns
  }

  return APP_OK;
}
