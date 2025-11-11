@echo off
cd /d "%~dp0.."

rem run tests
ctest --test-dir build

rem at this point you have .gcda/.gcno in build\
rem if you have gcov installed you can do, e.g.:
rem gcov src\main.cpp
rem gcov src\game.cpp
