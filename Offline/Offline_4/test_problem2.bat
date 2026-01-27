@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Compiling 2305007_Problem2.cpp...
echo ========================================
g++ -o 2305007_Problem2.exe 2305007_Problem2.cpp
if errorlevel 1 (
    echo Compilation failed!
    pause
    exit /b 1
)
echo Compilation successful!
echo.

set "passed=0"
set "failed=0"

echo ========================================
echo Running Test Cases...
echo ========================================
echo.

for %%i in (1 2 3 4 5) do (
    echo Testing Case %%i...
    
    rem Run the program with input and capture output
    2305007_Problem2.exe < "Test_Cases\Test_Cases\Problem 2\Inputs\%%i.txt" > temp_output.txt
    
    rem Get first line from actual output
    set "actual_line="
    for /f "usebackq delims=" %%a in ("temp_output.txt") do (
        if not defined actual_line set "actual_line=%%a"
    )
    
    rem Get first line from expected output
    set "expected_line="
    for /f "usebackq delims=" %%b in ("Test_Cases\Test_Cases\Problem 2\Outputs\%%i.txt") do (
        if not defined expected_line set "expected_line=%%b"
    )
    
    rem Compare first lines
    if "!actual_line!"=="!expected_line!" (
        echo [PASSED] Test Case %%i
        set /a passed+=1
    ) else (
        echo [FAILED] Test Case %%i
        echo Expected: !expected_line!
        echo Got:      !actual_line!
        echo ----------------------------------------
        set /a failed+=1
    )
    echo.
)

rem Clean up temporary file
del temp_output.txt

echo ========================================
echo Test Summary
echo ========================================
echo Passed: !passed!/5
echo Failed: !failed!/5
echo ========================================

pause
