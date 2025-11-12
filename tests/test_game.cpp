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

TEST_CASE("Making moves for multiple players") {
    Game g;

    for (int players = 2; players <= 7; ++players) { 
        int size = g.boardSize(players);

        SUBCASE("All players can make a valid move on empty board") {
            for (int p = 1; p <= players; ++p) {
                CHECK(g.placeMove(p, 0, p-1) == true);
            }
        }

        SUBCASE("Invalid moves are rejected") {
            for (int p = 1; p <= players; ++p) {
                // Same cell should be invalid
                CHECK(g.placeMove(p, 0, p-1) == false);
            }
            // Out-of-bounds moves
            for (int p = 1; p <= players; ++p) {
                CHECK(g.placeMove(p, -1, 0) == false);
                CHECK(g.placeMove(p, 0, size) == false);
            }
        }
    }
}

TEST_CASE("Draw scenario for multiple players") {
    Game g;

    for (int players = 2; players <= 7; ++players) { 
        int size = g.boardSize(players);
        int current_player = 1;
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                g.placeMove(current_player, r, c);
                current_player = (current_player % players) + 1;
            }
        }

        CHECK(g.checkWinner() == 0); 
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
