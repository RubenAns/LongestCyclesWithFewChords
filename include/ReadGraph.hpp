#ifndef READGRAPH_HPP
#define READGRAPH_HPP

#include <vector>
#include <string>
#include "bitset64.hpp"

// Interfaces for bitset-based graph reading (ReadGraphBitset.cpp)
void loadGraphBitset(std::string graphString, int numberOfVertices, bitset adjacencyList[]);

// Interfaces for matrix-based graph reading (ReadGraphMatrix.cpp)
void loadGraphMatrix(std::string graphString, int numberOfVertices, std::vector<std::vector<int>>& graph);

// Independent function to get the number of vertices.
// Implemented inline so it can be used by any translation unit without
// requiring linking a specific object file.
inline int getNumberOfVertices(std::string graphString) {
	if(graphString.size() == 0){
		printf("Error: String is empty.\n");
		abort();
	}
	else if((graphString[0] < 63 || graphString[0] > 126) && graphString[0] != '>') {
		printf("Error: Invalid start of graphstring %c.\n", graphString[0]);
		abort();
	}

	int index = 0;
	if (graphString[index] == '>') { // Skip >>graph6<< header.
		index += 10;
	}

	if(graphString[index] < 126) { // 0 <= n <= 62
		return (int) graphString[index] - 63;
	}

	else if(graphString[++index] < 126) {
		int number = 0;
		for(int i = 2; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else if (graphString[++index] < 126) {
		int number = 0;
		for (int i = 5; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else {
		printf("Error: Format only works for graphs up to 68719476735 vertices.\n");
		abort();
	}
}
#endif // READGRAPH_HPP
