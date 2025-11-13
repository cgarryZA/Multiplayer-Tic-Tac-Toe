// include/app.h

#pragma once
#include <functional>
#include <iosfwd>

int run_app(std::istream &in, std::ostream &out,
            const std::function<bool(int players)> &init_fn,
            const std::function<bool()> &turn_fn);
