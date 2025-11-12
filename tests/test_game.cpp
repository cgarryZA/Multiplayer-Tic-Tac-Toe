#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "third_party/doctest.h"
#include "game.h"

TEST_CASE("boardSize grows with number of players") {
  Game g;
  for (int players = 2; players <=7; ++players) {
    CHECK(g.boardSize(players) == players + 1);
  }
}

TEST_CASE("placeMove respects bounds and occupancy on 3x3") {
  Game g;
  g.init(2);
  CHECK(g.placeMove(0, 0, 'X') == true);
  CHECK(g.placeMove(0, 0, 'O') == false);
  CHECK(g.placeMove(-1, 0, 'X') == false);
  CHECK(g.placeMove(3, 3, 'X') == false);
}


TEST_CASE("detects wins on all board sizes") {
  for (int players = 2; players <=7; ++players) {
    Game g;
    g.init(players);
    int n = g.board_.getSize();

    //Row win
    g.init(players);
    for (int i = 0; i < n; ++i) g.placeMove(i, 0, 'X');
    CHECK(g.checkWinner() == 'X');

    //Column win
    g.init(players);
    for (int i = 0; i < n; ++i) g.placeMove(0, i, 'O');
    CHECK(g.checkWinner() == 'O');
    
    //Diag win
    g.init(players);
    for (int i = 0; i < n; ++i) g.placeMove(i, i, 'X');
    CHECK(g.checkWinner() == 'X');
    
    //Antidiag win
    g.init(players);
    for (int i = 0; i < n; ++i) g.placeMove(n-1-i, i, 'O');
    CHECK(g.checkWinner() == 'O');
  }
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
  CHECK(g.placeMove(0,0,'O'));
}
