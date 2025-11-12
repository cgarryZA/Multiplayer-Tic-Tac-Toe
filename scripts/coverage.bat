@echo off
cd /d "%~dp0.."

set COV_EXE="C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe"
if not exist %COV_EXE% (
    echo OpenCppCoverage not found. Install it with:
    echo   winget install OpenCppCoverage.OpenCppCoverage
    exit /b 1
)

ctest --test-dir build_debug -C Debug --output-on-failure

%COV_EXE% ^
  --export_type=html:coverage_html ^
  --sources="%CD%\src" ^
  -- "%CD%\build_debug\Debug\unit_tests.exe"
