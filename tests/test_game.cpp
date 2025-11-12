#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "third_party/doctest.h"
#include "game.h"

TEST_CASE("Making moves for multiple players") {
    Game g;
    g.init(5); 

   
    for (std::size_t p = 0; p < g.getPlayers().size(); ++p) {
        char playerSymbol = g.getPlayers()[p]; 
        CHECK(g.placeMove(0, p, playerSymbol) == true);
    }

  
    for (std::size_t p = 0; p < g.getPlayers().size(); ++p) {
        char playerSymbol = g.getPlayers()[p]; 
        CHECK(g.placeMove(0, p, playerSymbol) == false);
    }
}

TEST_CASE("Draw scenario for multiple players") {
    Game g;
    g.init(3);
    auto players = g.getPlayers();
    std::size_t size = g.getBoardSize();

  
    std::size_t idx = 0;
    for (std::size_t y = 0; y < size; ++y) {
        for (std::size_t x = 0; x < size; ++x) {
            g.placeMove(x, y, players[idx % players.size()]);
            ++idx;
        }
    }

    CHECK(g.checkWinner() == '\0');

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
