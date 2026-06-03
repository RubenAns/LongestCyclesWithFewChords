#ifndef COMPUTELONGESTCYCLEWITHFEWESTCHORDS_HPP
#define COMPUTELONGESTCYCLEWITHFEWESTCHORDS_HPP

#include "bitset64.hpp"

// Compute longest cycle with the fewest number of chords.
// Returns the length of the longest cycle.
// The cycle vertices are stored in longestCycleResult array.
int computeLongestCycleWithFewestChords(bitset* graph, const int n, int* longestCycleResult);
int computeLongestCycleWithFewestChordsWithHamFilter(bitset* graph, const int n, int* longestCycleResult);

#endif // COMPUTELONGESTCYCLEWITHFEWESTCHORDS_HPP
