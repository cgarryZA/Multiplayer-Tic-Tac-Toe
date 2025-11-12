// include/app.h

#pragma once
#include <iosfwd>
#include <functional>

// run_app is separated from main() so it can be unit-tested with
// doctest/CTest by injecting std::istringstream/std::ostringstream.
// This keeps main.cpp trivial and improves CI coverage.

int run_app(std::istream& in,
            std::ostream& out,
            const std::function<bool(int players)>& init_fn,
            const std::function<bool()>& turn_fn);
