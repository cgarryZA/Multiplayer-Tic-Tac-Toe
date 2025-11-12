@echo off
cd /d "%~dp0.."

if not exist build_debug (
    cmake -S . -B build_debug -DBUILD_TESTS=ON
    cmake --build build_debug --config Debug
)

build_debug\Debug\mttt.exe
