@echo off
cd /d "%~dp0.."

rem make sure it's built
if not exist build (
    cmake -S . -B build
    cmake --build build
)

rem run the executable CMake produced
build\mttt.exe
