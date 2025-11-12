@echo off
cd /d "%~dp0.."

rem run tests from the build dir, Debug config
ctest --test-dir build -C Debug --output-on-failure
