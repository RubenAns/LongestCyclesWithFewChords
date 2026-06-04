#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "bitset64.hpp"
#include "ReadGraph.hpp"

#define MAXVERTICES 64

void loadGraphBitset(std::string graphString, int numberOfVertices, bitset adjacencyList[]) { //Support only up to 128 vertices.
	int startIndex = 0;
	if (graphString[startIndex] == '>') { // Skip >>graph6<< header.
		startIndex += 10;
	}
	if (numberOfVertices <= 62) {
		startIndex += 1;
	}
	else if (numberOfVertices <= MAXVERTICES) {
		startIndex += 4;
	}
	else {
		printf("Error: Program can only handle graphs with %d vertices or fewer.\n", MAXVERTICES);
		abort();
	}

	for (int vertex = 0; vertex < numberOfVertices; vertex++) { //Initialize adjacencyList.
		adjacencyList[vertex] = EMPTY;
	}

	int currentVertex = 1;
	int sum = 0;
	for (int index = startIndex; index < (int) graphString.size() && graphString[index] != '\n'; index++) {
		int i;
		for (i = prev(graphString[index] - 63, 6); i != -1; i = prev(graphString[index] - 63, i)) {
			while (5 - i + (index - startIndex) * 6 - sum >= 0) {
				sum += currentVertex;
				currentVertex++;
			}
			sum -= --currentVertex;
			int neighbour = 5 - i + (index - startIndex) * 6 - sum;
			add(adjacencyList[currentVertex], neighbour);
			add(adjacencyList[neighbour], currentVertex);
		}
	}

}
#ifdef TEST_HELPER
//This code is meant as a test of this class and does not add any functionality
 int main(int argc, char *argv[]) {
 	// Read graph string from command-line argument when executing the program.
 	// If no argument is provided, fall back to the default graph string "C~".
 	const char *DEFAULT_GRAPH = "C~";
 	const char *graphString = NULL;
 	if (argc > 1 && argv[1] != NULL && argv[1][0] != '\0') {
 		graphString = argv[1];
 	} else {
 		graphString = DEFAULT_GRAPH;
 		printf("No graph string provided, using default '%s'\n", DEFAULT_GRAPH);
 	}

	int nVertices = getNumberOfVertices(graphString);
 	printf("Number of vertices: %d\n", nVertices);
 	bitset adjacencyList[nVertices];
 	loadGraphBitset(graphString,nVertices,adjacencyList);
 	for(int i = 0; i < nVertices; i++) {
 		printf("Neighbours of %d: %lu\n", i,adjacencyList[i]);
		std::cout<< std::bitset<64>(adjacencyList[i]).to_string() << std::endl;
 	}

	return 0;
 }
 #endif
