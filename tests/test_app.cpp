// tests/test_app.cpp

#include "third_party/doctest.h"
#include "app.h"
#include "codes.h"
#include <sstream>

static auto no_loop = []() {};

TEST_CASE("app: empty input -> error code 2") {
    std::istringstream in("");
    std::ostringstream out;
    int rc = run_app(in, out, no_loop);
    CHECK(rc == APP_INPUT_EOF);
    CHECK(out.str().find("Enter number of players:") != std::string::npos);
}

TEST_CASE("app: negative players -> error code 3") {
    std::istringstream in("-5\n");
    std::ostringstream out;
    int rc = run_app(in, out, no_loop);
    CHECK(rc == APP_INPUT_INVALID);
    CHECK(out.str().find("Invalid number of players.") != std::string::npos);
}

TEST_CASE("app: non-numeric input -> EOF branch") {
    std::istringstream in("hello\n");
    std::ostringstream out;
    int rc = run_app(in, out, no_loop);
    CHECK(rc == APP_INPUT_EOF);
    CHECK(out.str().find("Enter number of players:") != std::string::npos);
    CHECK(out.str().find("Input ended.") != std::string::npos);
}

TEST_CASE("app: zero players -> invalid") {
    std::istringstream in("0\n");
    std::ostringstream out;
    int rc = run_app(in, out, no_loop);
    CHECK(rc == APP_INPUT_INVALID);
    CHECK(out.str().find("Invalid number of players.") != std::string::npos);
}

TEST_CASE("app: leading whitespace before number") {
    std::istringstream in("   3\n");
    std::ostringstream out;
    int rc = run_app(in, out, no_loop);
    CHECK(out.str().find("Enter number of players:") != std::string::npos);
}

TEST_CASE("app: valid number -> returns OK when loop is injected") {
    std::istringstream in("2\n");
    std::ostringstream out;

    int rc = run_app(in, out, []() {
        // fake loop: do nothing
    });

    CHECK(rc == APP_OK);
    CHECK(out.str().find("Enter number of players:") != std::string::npos);
}
