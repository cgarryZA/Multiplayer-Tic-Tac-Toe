#include "third_party/doctest.h"
#include "game.h"


static std::vector<char> pull(Game &g, int n) {
    std::vector<char> out;
    out.reserve(n);
    for (int i = 0; i < n; ++i) out.push_back(g.nextScheduledPlayer());
    return out;
}

TEST_CASE("teams: 2 vs 1 -> chunks of size 2 for each team; solo repeats") {
    Game g;
    g.initWithPlayers({'A', 'B', 'C'});
    g.enableTeams({{'A', 'B'}, {'C'}});

    auto seq = pull(g, 6);
    std::vector<char> want = {'A', 'B', 'C', 'C', 'A', 'B'};
    CHECK(seq == want);
}

TEST_CASE("teams: 3,2,2 -> chunks of size 3; 2-member team repeats last") {
    Game g;
    g.initWithPlayers({'D', 'E', 'F', 'G', 'H', 'I', 'J'});

    g.enableTeams({{'D', 'E', 'F'}, {'G', 'H'}, {'I', 'J'}});

    auto seq = pull(g, 9);
    std::vector<char> want = {'D', 'E', 'F', 'G', 'H', 'G', 'I', 'J', 'I'};
    CHECK(seq == want);
}

TEST_CASE("teams do not change board sizing rule") {
    Game g;
    g.initWithPlayers({'A', 'B', 'C', 'D', 'E'});
    CHECK(Game::boardSize(5) == 6);
    g.enableTeams({{'A', 'B'}, {'C', 'D'}, {'E'}});
    CHECK(Game::boardSize(5) == 6);
}
