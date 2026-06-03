#ifndef COMPUTELONGESTCYCLEANDCOUNTTHEM_HPP
#define COMPUTELONGESTCYCLEANDCOUNTTHEM_HPP
#include <vector>

#include "bitset64.hpp"

// Compute longest cycle with the fewest number of chords.
// Returns the length of the longest cycle.
// The cycle vertices are stored in longestCycleResult array.
int computeLongestCycleAndCountThem(bitset* graph, const int n, int* longestCycleResult, int& length);
int computeLongestCycleMatrix(std::vector< std::vector<int>>& graph, std::vector<int>& longestCycleResult);

#endif // COMPUTELONGESTCYCLEANDCOUNTTHEM_HPP
