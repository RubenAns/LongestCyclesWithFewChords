#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "ReadGraph.hpp"
#include "ComputeLongestCycleWithFewestChords.hpp"
#include "CountChords.hpp"

//
// Find the longest cycle with the minimum number of chords.
// Returns the cycle as a vector of vertex indices.
//
std::vector<int> findLongestCycleMinChords(bitset* graph, int n) {
    int longestCycleResult[64];
    
    int cycleLength = computeLongestCycleWithFewestChords(graph, n, longestCycleResult);
    
    // Convert result to vector
    std::vector<int> result(longestCycleResult, longestCycleResult + cycleLength);
    
    return result;
}

//
// MAIN: Reads graph6 strings from stdin (one per line), finds longest cycle with min chords.
//
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    std::string graphString;
    
    while (std::getline(std::cin, graphString)) {
        if (graphString.empty()) continue;
        // Remove trailing CR if input uses CRLF (prevents stray '\r' in graphString)
        if (!graphString.empty() && graphString.back() == '\r')
            graphString.pop_back();
        try {
            // Parse graph6
            int n = getNumberOfVertices(graphString);
            if (n > 64) {
                std::cerr << "Skipping: Graph has " << n << " vertices, max supported is 64\n";
                continue;
            }
            
            bitset graph[64];
            loadGraphBitset(graphString, n, graph);
            
            // Find longest cycle with minimum chords
            std::vector<int> cycle = findLongestCycleMinChords(graph, n);
            
            // Count chords in this cycle
            int chordCount = countChords(graph, cycle.data(), cycle.size());
            
            // Output: graph6 cycleLength chordCount
            std::cout << graphString << " " << cycle.size() << " " << chordCount << "\n";
            
        } catch (const std::exception& e) {
            std::cerr << "Error processing " << graphString << ": " << e.what() << "\n";
            continue;
        }
    }
    return 0;
}
