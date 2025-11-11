@echo off
rem go to project root (one level up from this script)
cd /d "%~dp0.."

rem configure (only needed first time or when CMakeLists changes)
if not exist build (
    cmake -S . -B build -DENABLE_COVERAGE=ON
)

rem build
cmake --build build
