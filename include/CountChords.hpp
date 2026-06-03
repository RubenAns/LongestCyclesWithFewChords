#ifndef COUNTCHORDS_HPP
#define COUNTCHORDS_HPP

#include "bitset64.hpp"

// Compute the longest cycle in a graph represented as adjacency matrix (vector of vectors)
// Returns a vector with vertex indices in the cycle (order as implemented in source).
int countChords(bitset* graph, const int* longestCycle, const int cycle_length);
void printAndCountChords(bitset* graph, const int* longestCycle, const int cycle_length);


#endif // COUNTCHORDS_HPP
