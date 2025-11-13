// tests/test_game.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "game.h"
#include "third_party/doctest.h"
#include <iostream>
#include <sstream>
#ifndef ANALYZER_USE
#define ANALYZER_USE(x) (void)(x)
#endif

TEST_CASE("Making moves for multiple players") {
    Game g;
    g.init(5);

    constexpr char players[] = {'X', 'O', 'A', 'B', 'C'};

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

    constexpr char drawBoard[4][4] = {
        {'X', 'O', 'A', 'O'},
        {'O', 'A', 'X', 'A'},
        {'A', 'X', 'O', 'X'},
        {'O', 'A', 'X', 'O'}
    };

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

    constexpr char players[] = {'X', 'O', 'A'};
    int size = 4;

    for (int x = 0; x < size; ++x) {
        g.placeMove(x, 0, players[0]);
    }

    CHECK(g.checkWinner() == players[0]);
}

TEST_CASE("checkWinner detects column win") {
    Game g;
    g.init(3);

    constexpr char players[] = {'X', 'O', 'A'};
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

    CHECK(g.placeMove(2, 2, 'X') == true); // (2,2) valid only if size >= 3
    CHECK(g.placeMove(3, 3, 'O') == false); // (3,3) must be out of bounds
}

TEST_CASE("generateRandomSymbols skips bad board chars") {
    std::vector<char> extra = {'|'};
    auto symbols = Game::generateRandomSymbols(3, extra);

    for (char c: symbols) {
        CHECK(c != '|');
    }
}

TEST_CASE("placeMove rejects negative coordinates") {
    Game g;
    g.init(2);
    CHECK(g.placeMove(-1, 0, 'X') == false);
    CHECK(g.placeMove(0, -1, 'X') == false);
}

TEST_CASE("checkWinner detects main diagonal win") {
    Game g;
    g.init(2);

    CHECK(g.placeMove(0, 0, 'X'));
    CHECK(g.placeMove(1, 1, 'X'));
    CHECK(g.placeMove(2, 2, 'X'));

    CHECK(g.checkWinner() == 'X');
}

TEST_CASE("teamIndexOf handles mapped and unmapped players in teams mode") {
    Game g;
    g.initWithPlayers({'X', 'O', 'A'});
    g.enableTeams({{'X'}});

    CHECK(g.placeMove(0, 0, 'X'));
    CHECK(g.placeMove(1, 0, 'A'));
    CHECK(g.placeMove(2, 0, 'O'));
    CHECK(g.placeMove(3, 0, 'X'));

    for (int x = 0; x < 4; ++x) {
        CHECK(g.placeMove(x, 1, 'A'));
    }

    CHECK(g.checkWinner() == 'A');
}

TEST_CASE("nextScheduledPlayer cycles players in non-team mode") {
    Game g;
    g.initWithPlayers({'X', 'O', 'A'});

    CHECK(g.nextScheduledPlayer() == 'X');
    CHECK(g.nextScheduledPlayer() == 'O');
    CHECK(g.nextScheduledPlayer() == 'A');
    CHECK(g.nextScheduledPlayer() == 'X');
}

struct IORedirect {
    std::streambuf *cinOld;
    std::streambuf *coutOld;
    std::istringstream in;
    std::ostringstream out;

    explicit IORedirect(const std::string &input) : in(input) {
        cinOld = std::cin.rdbuf(in.rdbuf());
        coutOld = std::cout.rdbuf(out.rdbuf());
    }

    ~IORedirect() {
        std::cin.rdbuf(cinOld);
        std::cout.rdbuf(coutOld);
    }
};

TEST_CASE("playTurn returns false when input ends") {
    Game g;
    g.initWithPlayers({'X'});

    IORedirect io("");
    CHECK(g.playTurn() == false);
}

TEST_CASE("playTurn returns true on valid move with no winner and not full") {
    Game g;
    g.initWithPlayers({'X', 'O'});

    IORedirect io("0 0\n");
    CHECK(g.playTurn() == true);
}

TEST_CASE("playTurn returns true on invalid move") {
    Game g;
    g.initWithPlayers({'X'});
    g.placeMove(0, 0, 'X');

    IORedirect io("0 0\n");
    CHECK(g.playTurn() == true);
}

TEST_CASE("playTurn stops when player wins in non-team mode") {
    Game g;
    g.initWithPlayers({'X', 'O'});
    g.placeMove(0, 0, 'X');
    g.placeMove(1, 0, 'X');

    IORedirect io("2 0\n");
    CHECK(g.playTurn() == false);
}

TEST_CASE("playTurn stops when team wins in team mode") {
    Game g;
    g.initWithPlayers({'X', 'O'});
    g.enableTeams({{'X', 'O'}});
    g.placeMove(0, 0, 'X');
    g.placeMove(1, 0, 'O');

    IORedirect io("2 0\n");
    CHECK(g.playTurn() == false);
}

TEST_CASE("playTurn stops on draw when board becomes full") {
    Game g;
    g.initWithPlayers({'X', 'O'});

    g.placeMove(1, 0, 'X');
    g.placeMove(2, 0, 'O');
    g.placeMove(0, 1, 'O');
    g.placeMove(1, 1, 'X');
    g.placeMove(2, 1, 'X');
    g.placeMove(0, 2, 'X');
    g.placeMove(1, 2, 'O');
    g.placeMove(2, 2, 'O');

    IORedirect io("0 0\n");
    CHECK(g.playTurn() == false);
}
