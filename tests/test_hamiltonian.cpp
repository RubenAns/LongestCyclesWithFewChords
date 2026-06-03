#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

// Project headers
#include "ReadGraph.hpp"
#include "ComputeLongestCycleAndCountThem.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: test_hamiltonian <file_with_graph6> [--method Bitset or Matrix (default)] [--n: number_of_tests]\n";
        return 2;
    }
    int upper_bound = 200;
    bool methodIsMatrix=true;
    for (int i = 2; i < argc; ++i)
    {
        std::string arg = argv[i];


        if (arg == "--n" && i+1<argc){
            upper_bound = atoi(argv[i+1]);
        }
        if (arg == "--method" && i+1<argc){ 
            std::string arg2 = argv[i+1];
            if (arg2 == "Bitset") methodIsMatrix=false;
        }
    }
    std::cout << "using method Matrix?: "<< methodIsMatrix << std::endl;

    std::ifstream fh(argv[1]);
    if (!fh) { std::cerr << "Unable to open " << argv[1] << "\n"; return 2; }

    std::string line;
    int total = 0, failed = 0;

    while (std::getline(fh, line) && total < upper_bound) {
        if (line.empty() || line[0] == '#') continue;
        ++total;
        try {
            if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
            const int n = getNumberOfVertices(line);
            std::vector<int> cyc;
            int cyc2[64];
            int cycle_length;
            if (methodIsMatrix){
                std::vector<std::vector<int>> matrix;
                loadGraphMatrix(line, n, matrix);
                computeLongestCycleMatrix(matrix, cyc);
                cycle_length = (int)cyc.size() - 1;
            }
            else{
                bitset graph[64];
                std::cout << "\"" << line << "end\"" << std::endl;
                loadGraphBitset(line ,n, graph);
                computeLongestCycleAndCountThem(graph, n, cyc2, cycle_length);
            }
             // cycle contains repeated start at end in this impl
            if (cycle_length != n) {
                ++failed;
                std::cout << "FAIL: test "<<total<<": " << line << " n=" << n << " length=" << cycle_length << "\n";
            } else {
                std::cout << "OK: test "<<total<<": " << line << " n=" << n << " length=" << cycle_length << "\n";
            }
        } catch (const std::exception& e) {
            ++failed;
            std::cout << "ERROR: test "<<total<<": " << line << " -> " << e.what() << "\n";
        }
    
}
    std::cout << "Total=" << total << " Failed=" << failed << "\n";
    return failed ? 1 : 0;
}

