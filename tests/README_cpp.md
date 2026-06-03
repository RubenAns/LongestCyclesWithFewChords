C++ test harness
=================

This directory contains three C++ test programs that exercise your project's matrix-based implementation and compare results.

Files
- test_hamiltonian.cpp : reads graph6 strings and checks longest cycle == n-1
- test_longestpath.cpp : reads graph6 + expected length pairs
- test_compare.cpp : compares your implementation to an external executable by launching it and parsing its output
- Makefile : builds the three test executables

Build
From the tests directory (PowerShell):

    mingw32-make

Run

    .\test_hamiltonian.exe data\hamiltonian.txt
    .\test_longestpath.exe data\longestpath.txt
    .\test_compare.exe data\compare.txt ..\other.exe 0

Notes
- These programs call `loadGraphMatrix`, `getNumberOfVertices` and `computeLongestCycle` directly from your project sources. They therefore must be compiled with the project's .cpp files (the Makefile does this).
- The compare test launches an external exe and expects it to print the same textual lines your program does ("Graph has N vertices" and "Longest cycle length: L").
