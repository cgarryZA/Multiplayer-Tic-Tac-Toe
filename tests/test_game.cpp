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
  g.init(3);

  char players[] = {'X', 'O', 'A'};
  int size = 4;

  for (int i = 0; i < size; ++i) {
    g.placeMove(i, i, players[2]);
  }

  CHECK(g.checkWinner() == players[2]);
}
