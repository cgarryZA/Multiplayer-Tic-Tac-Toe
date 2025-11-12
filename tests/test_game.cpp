#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "third_party/doctest.h"
#include "game.h"

TEST_CASE("Making moves for multiple players") {
    Game g;
    g.init(5); // initialize with 5 players

    const auto& players = g.getPlayers();  // use getter

    // Each player makes a valid move on the first row
    for (std::size_t p = 0; p < players.size(); ++p) {
        char playerSymbol = players[p];
        CHECK(g.placeMove(0, p, playerSymbol) == true);
    }

    // Trying to place in the same cells should fail
    for (std::size_t p = 0; p < players.size(); ++p) {
        char playerSymbol = players[p];
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
}

TEST_CASE("checkWinner detects win on a full board") {
    Game g;
    g.init(3);
    auto players = g.getPlayers();
    std::size_t size = g.getBoardSize();

    for (std::size_t x = 0; x < size; ++x) {
        g.placeMove(x, 0, players[0]);
    }

    CHECK(g.checkWinner() == players[0]);
}
