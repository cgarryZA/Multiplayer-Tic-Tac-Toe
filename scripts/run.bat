@echo off
REM build.bat

REM make build dir if missing
if not exist build (
    mkdir build
)

REM configure + build
cmake -S . -B build
if errorlevel 1 (
    echo CMake configure failed.
    exit /b 1
)

cmake --build build
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

REM run the program
set EXE=build\mttt.exe
if exist "%EXE%" (
    echo Running %EXE% ...
    "%EXE%"
) else (
    echo Could not find %EXE%
)
