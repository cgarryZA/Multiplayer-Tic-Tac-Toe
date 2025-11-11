// test_main.cpp
#include "third_party/doctest.h"
#include <sstream>

int run_app(std::istream& in, std::ostream& out);

TEST_CASE("run_app handles no input") {
    std::istringstream in("");
    std::ostringstream out;
    int code = run_app(in, out);
    CHECK(code == 0);
}
