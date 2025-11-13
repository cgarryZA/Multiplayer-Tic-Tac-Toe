// tests/test_game.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "game.h"
#include "third_party/doctest.h"

TEST_CASE("Making moves for multiple players") {
  Game g;
  g.init(5);

  char players[] = {'X', 'O', 'A', 'B', 'C'};
  int boardSize = 6;

  for (int p = 0; p < 5; ++p) {
    CHECK(g.placeMove(p, 0, players[p]) == true);
  }

  for (int p = 0; p < 5; ++p) {
    CHECK(g.placeMove(p, 0, players[p]) == false);
  }
}

TEST_CASE("Draw scenario for multiple players") {
  Game g;
  g.init(3);

  char drawBoard[4][4] = {{'X', 'O', 'A', 'O'},
                          {'O', 'A', 'X', 'A'},
                          {'A', 'X', 'O', 'X'},
                          {'O', 'A', 'X', 'O'}};

  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      g.placeMove(x, y, drawBoard[y][x]);
    }
  }

  CHECK(g.checkWinner() == ' ');
}

TEST_CASE("checkWinner detects win on a full board") {
  Game g;
  g.init(3);

  char players[] = {'X', 'O', 'A'};
  int size = 4;

  for (int x = 0; x < size; ++x) {
    g.placeMove(x, 0, players[0]);
  }

  CHECK(g.checkWinner() == players[0]);
}

TEST_CASE("checkWinner detects column win") {
  Game g;
  g.init(3);

  char players[] = {'X', 'O', 'A'};
  int size = 4;

  for (int y = 0; y < size; ++y) {
    g.placeMove(0, y, players[1]);
  }

  CHECK(g.checkWinner() == players[1]);
}

TEST_CASE("checkWinner detects diagonal win") {
  Game g;
  g.init(2);
  CHECK(g.placeMove(0, 2, 'O'));
  CHECK(g.placeMove(1, 1, 'O'));
  CHECK(g.placeMove(2, 0, 'O'));
  CHECK(g.checkWinner() == 'O');
}

TEST_CASE("init clamps player count to MIN_PLAYERS") {
  Game g;
  g.init(1); // should clamp to 2 players → board size 3

  CHECK(g.placeMove(2, 2, 'X') == true);  // (2,2) valid only if size >= 3
  CHECK(g.placeMove(3, 3, 'O') == false); // (3,3) must be out of bounds
}

TEST_CASE("generateRandomSymbols skips bad board chars") {
  std::vector<char> extra = {'|'};
  auto symbols = Game::generateRandomSymbols(3, extra);

  for (char c : symbols) {
    CHECK(c != '|');
  }
}
