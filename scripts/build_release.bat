@echo off
rem build_release.bat - release build without tests

cd /d "%~dp0.."

rem configure release build in its own folder
if not exist build_release (
    cmake -S . -B build_release -DBUILD_TESTS=OFF
)

rem build Release config
cmake --build build_release --config Release
