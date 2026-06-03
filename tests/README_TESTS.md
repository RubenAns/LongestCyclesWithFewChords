Tests
=================

This directory contains three C++ test programs that test both the matrix-based implementation and the bitset implementation.

Files
- test_hamiltonian.cpp : reads graph6 strings and checks longest cycle == n-1
- test_longestpath.cpp : reads graph6 + expected length pairs
- test_countchords: reads graph6 strings and checks if #chords == |E|-|V| for hamiltonian graphs
- Makefile : builds the three test executables

Build
From the tests directory (PowerShell):

    mingw32-make

Run

    .\test_hamiltonian data\hamiltonian.txt or .\test_hamiltonian data\hamiltonian_sorted.txt
    .\test_longestpath data\longestpath.txt
    .\test_count_chords data\hamiltonian.txt  or .\test_hamiltonian data\hamiltonian_sorted.txt

Notes
    test_hamiltonian and test_longestpath has the options --method Bitset and --method Matrix to select the prefered graph representation. Bitset is selected by default.
    Each test also has the option to specify --n <number> to restrict the number of test cases to the first <number> of lines in the specified file.
