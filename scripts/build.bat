@echo off
rem build.bat - debug build with tests (and coverage if you want)

rem go to project root
cd /d "%~dp0.."

rem configure debug build in its own folder
if not exist build_debug (
    cmake -S . -B build_debug -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON
)

rem build Debug config (VS is multi-config, so use --config)
cmake --build build_debug --config Debug
