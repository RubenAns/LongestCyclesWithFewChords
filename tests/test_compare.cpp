#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include <chrono>

#include "ReadGraph.hpp"
#include "bitset64.hpp"
#include "ComputeLongestCycleAndCountThem.hpp"

#ifdef _WIN32
#include <windows.h>
#define POPEN _popen
#define PCLOSE _pclose
#else
#include <cstdio>
#define POPEN popen
#define PCLOSE pclose
#endif

// Run external exe (Jorik's blackbox) with a single graph6 via stdin; parse its two-line output
// Expected output per graph: line1 = graph6 (echo), line2 = number of distinct longest cycles
// Returns true on success and sets outCount.
static bool run_external_blackbox(const std::string& exePath, const std::string& extraArg, const std::string& graph6, long long& outCount) {
    // Write graph to a temporary file and redirect stdin
    const char* tmpFile = "temp_graph6.txt";
    {
        std::ofstream tmp(tmpFile);
        if (!tmp) return false;
        tmp << graph6 << "\n";
    }
    
    // Build command that reads from temp file
    std::string fullCmd = exePath;
    if (!extraArg.empty()) {
        fullCmd += " " + extraArg;
    }
    fullCmd += " < " + std::string(tmpFile);
    
    FILE* pipe = POPEN(fullCmd.c_str(), "r");
    if (!pipe) {
        remove(tmpFile);
        return false;
    }
    
    char buffer[4096];
    std::string out;
    while (fgets(buffer, sizeof(buffer), pipe)) out += buffer;
    PCLOSE(pipe);
    remove(tmpFile);
    
    // Debug: print what we captured
    if (out.empty()) {
        std::cerr << "DEBUG: No output captured from: " << fullCmd << "\n";
        return false;
    }
    
    // Parse: look for the second line being a number
    std::istringstream iss(out);
    std::string echoed;
    std::string numberLine;
    if (!std::getline(iss, echoed)) {
        std::cerr << "DEBUG: Failed to read first line. Output was:\n" << out << "\n";
        return false;
    }
    if (!std::getline(iss, numberLine)) {
        std::cerr << "DEBUG: Failed to read second line. First line: " << echoed << "\n";
        return false;
    }
    try {
        outCount = std::stoll(numberLine);
        return true;
    } catch (...) {
        std::cerr << "DEBUG: Failed to parse number from: '" << numberLine << "'\n";
        return false;
    }
}

int main(int argc, char** argv) {
    // Inputs: external exe path, optional arg to pass to external tool
    if (argc < 2) {
        std::cerr << "Usage: test_compare <external_exe_path> [external_arg]\n";
        std::cerr << "Reads graph6 lines from data/compare.txt\n";
        std::cerr << "Example: test_compare.exe countLongestCycles_Jorik.exe 0\n";
        return 2;
    }
    const std::string compareFile = "data/compare.txt";
    const std::string external = argv[1];
    const std::string externalArg = (argc >= 3) ? argv[2] : "";

    std::ifstream fh(compareFile);
    if (!fh) { std::cerr << "Unable to open " << compareFile << "\n"; return 2; }

    std::string line;
    int total = 0, failed = 0;
    bitset graph[64];
    int cyc2[64];

    // Timing accumulators
    auto t_start_mine = std::chrono::steady_clock::now();
    auto t_start_ext  = std::chrono::steady_clock::now();
    auto t_accum_mine = std::chrono::nanoseconds::zero();
    auto t_accum_ext  = std::chrono::nanoseconds::zero();

    while (std::getline(fh, line)) {
        if (line.empty() || line[0] == '#') continue;
        ++total;
        try {
            int n = getNumberOfVertices(line);
            if (n > 64) { std::cerr << "Skipping graph with n=" << n << " (>64)\n"; continue; }
            loadGraphBitset(line ,n, graph);

            // Time our implementation
            auto s1 = std::chrono::steady_clock::now();
            int my_num_cycles = computeLongestCycleAndCountThem(graph, n, cyc2);
            auto e1 = std::chrono::steady_clock::now();
            t_accum_mine += (e1 - s1);

            // Time external (Jorik) blackbox
            auto s2 = std::chrono::steady_clock::now();
            long long ext_count = 0;
            bool ok = run_external_blackbox(external, externalArg, line, ext_count);
            auto e2 = std::chrono::steady_clock::now();
            t_accum_ext += (e2 - s2);
            if (!ok) {
                ++failed;
                std::cout << "ERR_RUN: " << line << "\n";
                continue;
            }

            // We only compare cycle lengths; Jorik's tool reports number of distinct longest cycles
            // If you have a version that reports length, adapt parsing accordingly.
            // For now, we accept that both run and we print our length.
            if (my_num_cycles == ext_count){
                std::cout << "OK: " << line << " num_mine=" << my_num_cycles << "  count_ext=" << ext_count << "\n";
            }
            else{
                std::cout << "FAIL: " << line << " num_mine=" << my_num_cycles << "  count_ext=" << ext_count << "\n";
            }

        } catch (const std::exception& e) {
            ++failed;
            std::cout << "ERROR: " << line << " -> " << e.what() << "\n";
        }
    }

    std::cout << "Total=" << total << " Failed=" << failed << "\n";
    // Overall timing comparison (not per-graph):
    double ms_mine = std::chrono::duration_cast<std::chrono::milliseconds>(t_accum_mine).count();
    double ms_ext  = std::chrono::duration_cast<std::chrono::milliseconds>(t_accum_ext).count();
    std::cout << "Time_mine_ms=" << ms_mine << " Time_ext_ms=" << ms_ext << "\n";
    if (total > 0) {
        double ratio = (ms_ext > 0.0) ? (ms_mine / ms_ext) : 0.0;
        std::cout << "Overall speed ratio mine/ext=" << ratio << "\n";
    }

    return failed ? 1 : 0;
}
