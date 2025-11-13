// tests/test_app.cpp
#include "app.h"
#include "codes.h"
#include "third_party/doctest.h"
#include <sstream>
#ifndef ANALYZER_USE
#define ANALYZER_USE(x) (void)(x)
#endif

TEST_CASE("app: empty input -> error code EOF") {
  std::istringstream in("");
  std::ostringstream out;

  int rc = run_app(in, out, [](int) { return true; }, []() { return false; });
  ANALYZER_USE(rc);
  CHECK(rc == APP_INPUT_EOF);
  CHECK(out.str().find("Enter number of players:") != std::string::npos);
}

TEST_CASE("app: negative players -> invalid") {
  std::istringstream in("-5\n");
  std::ostringstream out;

  int rc = run_app(in, out, [](int) { return true; }, []() { return false; });
  ANALYZER_USE(rc);
  CHECK(rc == APP_INPUT_INVALID);
  CHECK(out.str().find("Invalid number of players.") != std::string::npos);
}

TEST_CASE("app: non-numeric input -> EOF branch") {
  std::istringstream in("hello\n");
  std::ostringstream out;

  int rc = run_app(in, out, [](int) { return true; }, []() { return false; });
  ANALYZER_USE(rc);
  CHECK(rc == APP_INPUT_EOF);
  CHECK(out.str().find("Input ended.") != std::string::npos);
}

TEST_CASE("app: zero players -> invalid") {
  std::istringstream in("0\n");
  std::ostringstream out;

  int rc = run_app(in, out, [](int) { return true; }, []() { return false; });
  ANALYZER_USE(rc);
  CHECK(rc == APP_INPUT_INVALID);
}

TEST_CASE("app: valid players -> calls init and one turn") {
  std::istringstream in("2\n");
  std::ostringstream out;

  bool init_called = false;
  int turn_count = 0;

  int rc = run_app(
      in, out,
      [&](int players) {
        init_called = true;
        CHECK(players == 2);
        return true;
      },
      [&]() {
        turn_count++;
        return false; // stop immediately
      });
  ANALYZER_USE(rc);
  CHECK(rc == APP_OK);
  CHECK(init_called == true);
  CHECK(turn_count == 1);
}

TEST_CASE("app: init function returns false -> Failed to init") {
  std::istringstream in("2\n");
  std::ostringstream out;

  int rc = run_app(
      in, out, [](int) { return false; }, // simulate init failure
      []() { return false; }              // no turns
  );
  ANALYZER_USE(rc);
  CHECK(rc == APP_INPUT_INVALID);
  CHECK(out.str().find("Failed to init.") != std::string::npos);
}

TEST_CASE("app: loop runs at least once") {
  std::istringstream in("2\n");
  std::ostringstream out;

  bool first = true;
  int rc = run_app(
      in, out, [](int) { return true; },
      [&]() {
        if (first) {
          first = false;
          return true;
        }
        return false;
      });
  ANALYZER_USE(rc);
  CHECK(rc == APP_OK);
}
