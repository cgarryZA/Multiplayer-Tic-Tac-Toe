// tests/test_game.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "third_party/doctest.h"

#include "game.h"
// #include "board.h" // include later

TEST_CASE("boardSize grows with number of players") {
    CHECK(Game::boardSize(2) == 3);
    CHECK(Game::boardSize(3) == 4);
}

TEST_CASE("placeMove respects bounds and occupancy on 3x3") {
    Game g;
    g.init(2);
    CHECK(g.placeMove(0,  0, 'X')  == true);
    CHECK(g.placeMove(0,  0, 'O')  == false);
    CHECK(g.placeMove(-1, 0, 'X')  == false);
    CHECK(g.placeMove(3,  3, 'X')  == false);
}

TEST_CASE("checkWinner detects a simple row win") {
    Game g;
    g.init(2);
    CHECK(g.placeMove(0, 0,  'X'));
    CHECK(g.placeMove(1, 0,  'X'));
    CHECK(g.placeMove(2, 0,  'X'));
    CHECK(g.checkWinner() == 'X');
}
