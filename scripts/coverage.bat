@echo off
cd /d "%~dp0.."
ctest --test-dir build_debug -C Debug --output-on-failure
