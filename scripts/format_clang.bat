@echo off
setlocal

if "%~1"=="" (
  set "CLANG_FORMAT=clang-format"
) else (
  set "CLANG_FORMAT=%~1"
)

rem go to repo root (parent of scripts)
cd /d "%~dp0.."

for %%D in (tests src include) do (
  if exist "%%D" (
    pushd "%%D"
    for %%F in (*.*) do "%CLANG_FORMAT%" -i "%%F"
    popd
  )
)

endlocal
