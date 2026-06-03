#include <iostream>
#include <ctime>
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
    // std::cout<<"Finding longest cycle with min chords with ham filter..."<<std::endl;
    int longestCycleResult[64];
    
    // Compute longest cycle and collect all candidates with chord counts
    int cycleLength = computeLongestCycleWithFewestChordsWithHamFilter(graph, n, longestCycleResult);
    if (cycleLength == n) {
        // Graph is Hamiltonian, return empty cycle
        // std::cout << "Graph is Hamiltonian, skipping output.\n";
        return std::vector<int>();
    }
    // Convert result to vector
    std::vector<int> result(longestCycleResult, longestCycleResult + cycleLength);
    
    return result;
}

//
// MAIN: Reads graph6 strings from stdin (one per line), finds longest cycle with min chords.
//
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    const std::clock_t cpu_start = std::clock();
    int number_of_graphs_read = 0;
    int number_of_ham_graphs = 0;

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    std::string graphString;
    
    while (std::getline(std::cin, graphString)) {
        if (graphString.empty()) continue;
        // Remove trailing CR if input uses CRLF (prevents stray '\r' in graphString)
        if (!graphString.empty() && graphString.back() == '\r')
            graphString.pop_back();
        
        try {
            number_of_graphs_read++;
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
            
            if (cycle.size() == 0) {
                number_of_ham_graphs++;
                // Graph is Hamiltonian, skip output
                continue;
            }
            // Count chords in this cycle
            int chordCount = countChords(graph, cycle.data(), cycle.size());
            
            // Output: graph6 cycleLength chordCount
            std::cout << graphString << " " << cycle.size() << " " << chordCount;
            std::cout <<" [";
            for (int v: cycle)
                std::cout << v << ' ';
            std::cout <<"] \n";
            
        } catch (const std::exception& e) {
            std::cerr << "Error processing " << graphString << ": " << e.what() << "\n";
            continue;
        }
    }
    const double cpu_seconds = static_cast<double>(std::clock() - cpu_start) / CLOCKS_PER_SEC;
    std::cerr << "Graphs read: "<< number_of_graphs_read << "\nNon-ham graphs written: " << number_of_graphs_read-number_of_ham_graphs << std::endl;
    std::cerr << "CPU time: " << cpu_seconds << "s" << std::endl;
    return 0;
}
