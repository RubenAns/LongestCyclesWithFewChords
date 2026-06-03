#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

// Project headers
#include "ReadGraph.hpp"
#include "ComputeLongestCycleAndCountThem.hpp"
#include "CountChords.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: test_count_chords <file_with_hamiltonian_graphs> [--n: number_of_tests]\n";
        return 2;
    }
    int upper_bound = 200;
    
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--n" && i+1 < argc) {
            upper_bound = atoi(argv[i+1]);
        }
    }

    std::ifstream fh(argv[1]);
    if (!fh) { 
        std::cerr << "Unable to open " << argv[1] << "\n"; 
        return 2; 
    }

    std::string line;
    int total = 0, failed = 0;

    while (std::getline(fh, line) && total < upper_bound) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
            // std::cout << line << std::endl;

        if (line.empty() || line[0] == '#') continue;
        ++total;
        try {
            // std::cout << line[0] <<std::endl;
            // std::cout << "begin" + line + "end" << std::endl;
            const int n = getNumberOfVertices(line);
            bitset graph[64];
            loadGraphBitset(line, n, graph);
            
            // Count edges in the graph
            int edge_count = 0;
            for (int i = 0; i < n; i++) {
                edge_count += bitset_size(graph[i]);
            }
            edge_count /= 2; // Each edge is counted twice
            
            // Expected chords for Hamiltonian graph: |E| - |V|
            int expected_chords = edge_count - n;
            
            // Find Hamiltonian cycle
            int cycle[64];
            for (int i = 0; i < 64; ++i) {
                cycle[i] = -1;
            }
            // std::cout <<"calculating cycle" <<std::endl;
            // std::cout << graph << n <<std::endl;
            // for (int i=0; i<n; i++){
            //      std::cout << graph[i] <<" ";
            // }
            // std::cout << std::endl;
            int cycle_length = 0;

            computeLongestCycleAndCountThem(graph, n, cycle, cycle_length);

            // std::cout << cycle_length <<std::endl;
            if (cycle_length != n) {
                ++failed;
                std::cout << "FAIL: test " << total << ": " << line 
                          << " - Not Hamiltonian (cycle length=" << cycle_length << ", |V|=" << n << ")\n";
            } else {
                // Count actual chords
                int actual_chords = countChords(graph, cycle, cycle_length);
                
                if (actual_chords != expected_chords) {
                    ++failed;
                    std::cout << "FAIL: test " << total << ": " << line 
                              << " |V|=" << n << " |E|=" << edge_count 
                              << " expected_chords=" << expected_chords 
                              << " actual_chords=" << actual_chords << "\n";
                } else {
                    std::cout << "OK: test " << total << ": " << line 
                              << " |V|=" << n << " |E|=" << edge_count 
                              << " chords=" << actual_chords << "\n";
                }
            }
        } catch (const std::exception& e) {
            ++failed;
            std::cout << "ERROR: test " << total << ": " << line << " -> " << e.what() << "\n";
        }
    }
    
    std::cout << "Total=" << total << " Failed=" << failed << "\n";
    return failed ? 1 : 0;
}
