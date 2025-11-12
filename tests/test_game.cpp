#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "third_party/doctest.h"
#include "game.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "third_party/doctest.h"
#include "game.h"

TEST_CASE("Making moves for multiple players") {
    Game g;
    g.init(5); // 5 players

    // First 5 symbols based on Game logic:
    // X, O, and next three from generateRandomSymbols
    char players[] = {'X', 'O', 'A', 'B', 'C'};
    int boardSize = 6; // numPlayers + 1

    // Each player makes a valid move in a separate row of the first column
    for (int p = 0; p < 5; ++p) {
        CHECK(g.placeMove(p, 0, players[p]) == true);
    }

    // Same cells cannot be reused
    for (int p = 0; p < 5; ++p) {
        CHECK(g.placeMove(p, 0, players[p]) == false);
    }
}

TEST_CASE("Draw scenario for multiple players") {
    Game g;
    g.init(3); // 3 players

    char players[] = {'X', 'O', 'A'};
    int size = 4; // 3 + 1

    int idx = 0;
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            g.placeMove(x, y, players[idx % 3]);
            ++idx;
        }
    }

    // Game returns ' ' (space) when no winner
    CHECK(g.checkWinner() == ' ');
}

TEST_CASE("checkWinner detects win on a full board") {
    Game g;
    g.init(3); // 3 players

    char players[] = {'X', 'O', 'A'};
    int size = 4; // 3 + 1

    // Fill first row with player 0 (X)
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

    // Fill first column with player 1 (O)
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

    // Fill main diagonal with player 2 (A)
    for (int i = 0; i < size; ++i) {
        g.placeMove(i, i, players[2]);
    }

    CHECK(g.checkWinner() == players[2]);
}
