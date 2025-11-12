#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "third_party/doctest.h"
#include "game.h"

TEST_CASE("Making moves for multiple players") {
    Game g;
    g.init(5); // 5 players

    const auto& players = g.getPlayers();
    std::size_t boardSize = g.getBoardSize();

    // Each player makes a valid move on first column
    for (std::size_t p = 0; p < players.size(); ++p) {
        char playerSymbol = players[p];
        CHECK(g.placeMove(p, 0, playerSymbol) == true);
    }

    // Same cells cannot be reused
    for (std::size_t p = 0; p < players.size(); ++p) {
        char playerSymbol = players[p];
        CHECK(g.placeMove(p, 0, playerSymbol) == false);
    }
}

TEST_CASE("Draw scenario for multiple players") {
    Game g;
    g.init(3);
    const auto& players = g.getPlayers();
    std::size_t size = g.getBoardSize();

    std::size_t idx = 0;
    for (std::size_t y = 0; y < size; ++y) {
        for (std::size_t x = 0; x < size; ++x) {
            g.placeMove(x, y, players[idx % players.size()]);
            ++idx;
        }
    }

    // Game returns ' ' (space) when no winner
    CHECK(g.checkWinner() == ' ');
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
