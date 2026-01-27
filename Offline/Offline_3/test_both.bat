@echo off
setlocal enabledelayedexpansion
REM Comprehensive test script for Prim's (Problem1) and Kruskal's (Problem2)
echo ========================================
echo MST Implementation Test Suite
echo ========================================
echo.

REM Compile both programs
echo [COMPILATION] Building programs...
echo.
echo Building Problem1 (Prim's)...
C:\msys64\mingw64\bin\g++.exe -fdiagnostics-color=always -g 2305007_Problem1.cpp -o prim.exe
if !errorlevel! neq 0 (
    echo.
    echo [ERROR] Failed to compile Problem1 (Prim's)
    echo Trying alternative compiler...
    g++.exe -fdiagnostics-color=always -g 2305007_Problem1.cpp -o prim.exe
    if !errorlevel! neq 0 (
        pause
        exit /b 1
    )
)

echo.
echo Building Problem2 (Kruskal's)...
C:\msys64\mingw64\bin\g++.exe -fdiagnostics-color=always -g 2305007_Problem2.cpp -o kruskal.exe
if !errorlevel! neq 0 (
    echo.
    echo [ERROR] Failed to compile Problem2 (Kruskal's)
    echo Trying alternative compiler...
    g++.exe -fdiagnostics-color=always -g 2305007_Problem2.cpp -o kruskal.exe
    if !errorlevel! neq 0 (
        pause
        exit /b 1
    )
)
echo.
echo [OK] Both programs compiled successfully!
echo.

REM Initialize counters
set primPassed=0
set primFailed=0
set kruskalPassed=0
set kruskalFailed=0

REM ==================== TEST PRIM'S ALGORITHM ====================
echo ==================== PRIM'S ALGORITHM (Problem1) ====================
echo.

for /L %%i in (1,1,10) do (
    .\prim.exe < sampleio\sampleio\input\test%%i.txt > prim_out%%i.txt 2>&1
    powershell -NoProfile -ExecutionPolicy Bypass -File compare.ps1 -testNum %%i -algorithm prims -program prim
    if !errorlevel! equ 0 (
        set /a primPassed+=1
    ) else (
        set /a primFailed+=1
    )
)

echo.
echo ==================== KRUSKAL'S ALGORITHM (Problem2) ====================
echo.

for /L %%i in (1,1,10) do (
    .\kruskal.exe < sampleio\sampleio\input\test%%i.txt > kruskal_out%%i.txt 2>&1
    powershell -NoProfile -ExecutionPolicy Bypass -File compare.ps1 -testNum %%i -algorithm kruskals -program kruskal
    if !errorlevel! equ 0 (
        set /a kruskalPassed+=1
    ) else (
        set /a kruskalFailed+=1
    )
)

echo.
echo ========================================
echo FINAL VERDICT
echo ========================================
echo.
echo PRIM'S ALGORITHM (Problem1):
if !primFailed! equ 0 (
    echo   Status: ALL TESTS PASSED [OK]
    echo   Result: !primPassed!/10 tests passed
) else (
    echo   Status: SOME TESTS FAILED
    echo   Result: !primPassed!/10 tests passed, !primFailed!/10 failed
)

echo.
echo KRUSKAL'S ALGORITHM (Problem2):
if !kruskalFailed! equ 0 (
    echo   Status: ALL TESTS PASSED [OK]
    echo   Result: !kruskalPassed!/10 tests passed
) else (
    echo   Status: SOME TESTS FAILED
    echo   Result: !kruskalPassed!/10 tests passed, !kruskalFailed!/10 failed
)

echo.
set /a totalPassed=!primPassed!+!kruskalPassed!
set /a totalFailed=!primFailed!+!kruskalFailed!
echo OVERALL:
if !totalFailed! equ 0 (
    echo   Status: EXCELLENT - ALL 20 TESTS PASSED
) else if !totalFailed! leq 2 (
    echo   Status: GOOD - Most tests passed
    echo   Total: !totalPassed!/20 passed
) else (
    echo   Status: NEEDS REVIEW - Multiple failures
    echo   Total: !totalPassed!/20 passed, !totalFailed!/20 failed
)
echo.
echo ========================================
pause
