#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

#include "GraphAPI.hpp"
#include "ComputeLongestCycle_copy.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: test_longestpath <file_with_graph6_and_expected>  [--method Bitset or Matrix (default)]\n";
        return 2;
    }
    bool methodIsMatrix = true;
    for (int i = 2; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--method" && i + 1 < argc)
        {
            std::string arg2 = argv[i + 1];
            if (arg2 == "Bitset")
                methodIsMatrix = false;
        }
    }
    std::cout << "using method Matrix?: " << methodIsMatrix << std::endl;

    std::ifstream fh(argv[1]);
    if (!fh)
    {
        std::cerr << "Unable to open " << argv[1] << "\n";
        return 2;
    }

    std::string line;
    int total = 0, failed = 0;
    while (std::getline(fh, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
        std::istringstream iss(line);
        std::string g;
        int expected;
        if (!(iss >> g >> expected))
            continue;
        ++total;
        try
        {
            int n = getNumberOfVertices(g);
            std::vector<int> cyc;
            int cyc2[64];
            int cycle_length;
            if (methodIsMatrix)
            {
                std::vector<std::vector<int>> matrix;
                loadGraphMatrix(g, n, matrix);
                cyc = computeLongestCycle(matrix);
                cycle_length = (int)cyc.size() - 1;
            }
            else
            {
                bitset graph[64];
                // std::cout<<n<<std::endl;
                loadGraphBitset(g, n, graph);
                // printMatrix(bitsetToMatrix(graph,7));
                // for (size_t i = 0; i < n; i++)
                // {
                //     std::cout<<graph[i] << ", ";
                // }
                // std::cout<<std::endl;
                cycle_length = computeLongestCycle_copy(graph, n, cyc2);
            }
            if (cycle_length != expected)
            {
                ++failed;
                std::cout << "FAIL: " << g << " expected=" << expected << " got=" << cycle_length << "\n";
            }
            else
            {
                std::cout << "OK: " << g << " expected=" << expected << " got=" << cycle_length << "\n";
            }
        }
        catch (const std::exception &e)
        {
            ++failed;
            std::cout << "ERROR: " << g << " -> " << e.what() << "\n";
        }
    }
    std::cout << "Total=" << total << " Failed=" << failed << "\n";
    return failed ? 1 : 0;
}
