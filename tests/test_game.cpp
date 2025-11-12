#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "third_party/doctest.h"

#include "game.h"
// #include "board.h" // include later

//TEST_CASE("boardSize grows with number of players") {
  //CHECK(Game::boardSize(2) == 3);
  //CHECK(Game::boardSize(3) == 4);
  //CHECK(Game::boardSize(4) == 5);
  //CHECK(Game::boardSize(5) == 6);
  //CHECK(Game::boardSize(6) == 7);
//}

TEST_CASE("boardSize grows with number of players") {
  for (int players = 2; players <=7; ++players) {
    Game g;
    g.init(players);
    std::size_t size = g.checkWinner();
    CHECK(g.boardSize(players) == g.board_.getSize());
    CHECK(g.board_.getSize() == players + 1);
}

TEST_CASE("placeMove respects bounds and occupancy on 3x3") {
  Game g;
  g.init(2);
  CHECK(g.placeMove(0, 0, 'X') == true);
  CHECK(g.placeMove(0, 0, 'O') == false);
  CHECK(g.placeMove(-1, 0, 'X') == false);
  CHECK(g.placeMove(3, 3, 'X') == false);
}

TEST_CASE("checkWinner detects a simple row win") {
  Game g;
  g.init(2);
  CHECK(g.placeMove(0, 0, 'X'));
  CHECK(g.placeMove(1, 0, 'X'));
  CHECK(g.placeMove(2, 0, 'X'));
  CHECK(g.checkWinner() == 'X');
}

TEST_CASE("checkWinner detects a simple column win") {
  Game g;
  g.init(2);
  CHECK(g.placeMove(1, 0, 'O'));
  CHECK(g.placeMove(1, 1, 'O'));
  CHECK(g.placeMove(1, 2, 'O'));
  CHECK(g.checkWinner() == 'O');
}

TEST_CASE("checkWinner detects a diagonal win") {
  Game g;
  g.init(2);
  CHECK(g.placeMove(0, 0, 'X'));
  CHECK(g.placeMove(1, 1, 'X'));
  CHECK(g.placeMove(2, 2, 'X'));
  CHECK(g.checkWinner() == 'X');
}

TEST_CASE("checkWinner detects an antidiagonal win") {
  Game g;
  g.init(2);
  CHECK(g.placeMove(0, 2, 'O'));
  CHECK(g.placeMove(1, 1, 'O'));
  CHECK(g.placeMove(2, 0, 'O'));
  CHECK(g.checkWinner() == 'O');
}

TEST_CASE("checkWinner returns none when there is no win yet") {
  Game g;
  g.init(2);
  g.placeMove(0, 0, 'X');
  g.placeMove(0, 0, 'O');
  g.placeMove(2, 2, 'X');
  CHECK(g.checkWinner() == '\0');
}

TEST_CASE("checkWinner returns none when there is no win yet on a full board") {
  Game g;
  g.init(2);
  g.placeMove(0, 0, 'X');
  g.placeMove(0, 1, 'O');
  g.placeMove(0, 2, 'X');
  g.placeMove(1, 0, 'O');
  g.placeMove(1, 2, 'O');
  g.placeMove(2, 0, 'X');
  g.placeMove(2, 1, 'O');
  g.placeMove(1, 1, 'X');
  g.placeMove(2, 2, 'X');
  CHECK(g.checkWinner() == '\0');
}

TEST_CASE("checkWinner detects win on a full board") {
  Game g;
  g.init(2);
  g.placeMove(0, 0, 'X');
  g.placeMove(0, 1, 'O');
  g.placeMove(0, 2, 'X');
  g.placeMove(1, 0, 'O');
  g.placeMove(1, 2, 'O');
  g.placeMove(2, 0, 'X');
  g.placeMove(2, 1, 'O');
  g.placeMove(1, 1, 'X');
  g.placeMove(2, 2, 'X');
  CHECK(g.checkWinner() == '\0');
}

TEST_CASE("Init clears previous moves") {
  Game g;
  g.init(2);
  g.placeMove(0,0,'X');
  g.init(2);
  CHECK(g.plaveMove(0,0,'O'));
}
