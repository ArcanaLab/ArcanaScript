@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

REM Get the base path (project root)
set BASE_PATH=%~dp0
set BASE_PATH=%BASE_PATH:~0,-1%
set BASE_PATH=%BASE_PATH%\..\..

REM Check for parameter
if "%1"=="" (
    echo Usage: %0 ^<test_number^> [a^|r]
    echo Example: %0 64 a
    echo.
    echo Available accept tests:
    dir /b %BASE_PATH%\src\test\c\accept | more
    echo.
    echo Available reject tests:
    dir /b %BASE_PATH%\src\test\c\reject | more
    goto :eof
)

set TEST_NUM=%1
set TEST_MODE=%2
set FOUND_FILE=
set TEST_TYPE=

if /I "%TEST_MODE%"=="a" (
    REM Search only in accept
    for %%f in (%BASE_PATH%\src\test\c\accept\%TEST_NUM%-*.arcx) do (
        set FOUND_FILE=%%f
        set TEST_TYPE=accept
        goto :found
    )
    echo Error: Accept test file starting with '%TEST_NUM%-' not found.
    echo.
    echo Available accept tests:
    dir /b %BASE_PATH%\src\test\c\accept | more
    goto :eof
) else if /I "%TEST_MODE%"=="r" (
    REM Search only in reject
    for %%f in (%BASE_PATH%\src\test\c\reject\%TEST_NUM%-*.arcx) do (
        set FOUND_FILE=%%f
        set TEST_TYPE=reject
        goto :found
    )
    echo Error: Reject test file starting with '%TEST_NUM%-' not found.
    echo.
    echo Available reject tests:
    dir /b %BASE_PATH%\src\test\c\reject | more
    goto :eof
) else (
    REM Search in accept
    for %%f in (%BASE_PATH%\src\test\c\accept\%TEST_NUM%-*.arcx) do (
        set FOUND_FILE=%%f
        set TEST_TYPE=accept
        goto :found
    )
    REM Search in reject
    for %%f in (%BASE_PATH%\src\test\c\reject\%TEST_NUM%-*.arcx) do (
        set FOUND_FILE=%%f
        set TEST_TYPE=reject
        goto :found
    )
    echo Error: Test file starting with '%TEST_NUM%-' not found in accept or reject directories.
    echo.
    echo Available accept tests:
    dir /b %BASE_PATH%\src\test\c\accept | more
    echo.
    echo Available reject tests:
    dir /b %BASE_PATH%\src\test\c\reject | more
    goto :eof
)

:found
echo === Testing %TEST_TYPE% file: !FOUND_FILE! ===
type "!FOUND_FILE!" | %BASE_PATH%\build\Debug\Compiler.exe "!FOUND_FILE!"
set RESULT=!ERRORLEVEL!

if "%TEST_TYPE%"=="accept" (
    if !RESULT! equ 0 (
        echo     !FOUND_FILE!, accepted correctly (status !RESULT!)
    ) else (
        echo     !FOUND_FILE!, rejected incorrectly (status !RESULT!)
    )
) else (
    if !RESULT! neq 0 (
        echo     !FOUND_FILE!, rejected correctly (status !RESULT!)
    ) else (
        echo     !FOUND_FILE!, accepted incorrectly (status !RESULT!)
    )
)

ENDLOCAL 