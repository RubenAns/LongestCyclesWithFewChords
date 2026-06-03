#ifndef GRAPH_API_HPP
#define GRAPH_API_HPP

/**
 * @file GraphAPI.hpp
 * @brief Umbrella header for graph reading and longest cycle computation.
 *
 * This header provides a unified interface for reading graphs and computing
 * the longest cycle using either bitset or adjacency matrix representations.
 *
 * Supported APIs:
 *
 * 1. BITSET PATH (link ReadGraphBitset.o + ComputeLongestCycleBitset.o):
 *    - int getNumberOfVertices(const char* graphString)
 *    - void loadGraphBitset(std::string graphString, int n, bitset adjacencyList[])
 *    - std::vector<int> computeLongestCycle(bitset* graph, const int n)
 *
 * 2. MATRIX PATH (link ReadGraphMatrix.o + ComputeLongestCycleMatrix.o):
 *    - int getNumberOfVertices(std::string graphString)
 *    - void loadGraphMatrix(std::string graphString, int n, std::vector<std::vector<int>>& graph)
 *    - std::vector<int> computeLongestCycle(std::vector<std::vector<int>>& graph)
 *
 * 3. CONVERSION HELPERS (no extra linking required):
 *    - std::vector<std::vector<int>> bitsetToMatrix(const bitset* adj, int n)
 *    - Helper functions to bridge between representations.
 *
 * Example usage (bitset path):
 *   #include "GraphAPI.hpp"
 *   int n = getNumberOfVertices("C~");
 *   bitset graph[n];
 *   loadGraphBitset("C~", n, graph);
 *   auto cycle = computeLongestCycle(graph, n);
 *
 * Example usage (matrix path):
 *   #include "GraphAPI.hpp"
 *   int n = getNumberOfVertices("C~");
 *   std::vector<std::vector<int>> graph;
 *   loadGraphMatrix("C~", n, graph);
 *   auto cycle = computeLongestCycle(graph);
 *
 * Makefile integration:
 *   For bitset implementations: link ComputeLongestCycleBitset.o + ReadGraphBitset.o + ReadGraphMatrix.o
 *   For matrix implementations:  link ComputeLongestCycleMatrix.o + ReadGraphMatrix.o
 */

#include "bitset64.hpp"
#include "ReadGraph.hpp"
#include "ComputeLongestCycle.hpp"
#include <iostream>


// Convert bitset adjacency list to adjacency matrix
inline std::vector<std::vector<int>> bitsetToMatrix(const bitset* adjacencyList, int n) {
	std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (contains(adjacencyList[i], j)) {
				matrix[i][j] = 1;
			}
		}
	}
	return matrix;
}

// Pretty-print an adjacency matrix to an output stream
inline void printMatrix(const std::vector<std::vector<int>> &matrix) {
	std::cout << "[\n";
	for (const auto &row : matrix) {
		std::cout << "  [ ";
		for (const auto &v : row) std::cout << v << " ";
		std::cout << "]\n";
	}
	std::cout << "]\n";
}

template <typename A>
inline void printArray(const A* array, const int size){
		std::cout<<"[";
		for (int i = 0; i < size-1; i++)
		{
			std::cout<<array[i]<<", ";
		}
		std::cout<<array[size-1]<<"]"<<std::endl;
}

// } // namespace graphapi

#endif