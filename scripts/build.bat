cmake -S . -B build -DENABLE_COVERAGE=ON
cmake --build build
cd build
ctest
