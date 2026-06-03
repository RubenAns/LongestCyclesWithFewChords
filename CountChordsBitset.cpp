#include "ComputeLongestCycle_copy.hpp"
#include <iostream>
#include <cstring>
#include "bitset64.hpp"
#include <cassert>
#include "GraphAPI.hpp"
using namespace std;
#define maxVertices 64

int countChords(bitset* graph, const int size_of_graph, const int* longestCycle, const int cycle_length){
    //convert cycle to bitset
    bitset cycleContains = EMPTY;
    int count = 0;
    for (size_t i = 0; i < cycle_length; i++)
    {
        add(cycleContains, longestCycle[i]);
    }
    forEach(i, cycleContains)
    {
        // printMatrix(bitsetToMatrix(graph, size_of_graph));
        // printArray(graph, size_of_graph);
        // std::cout<<cycleContains<<", ";
        // std::cout<<graph[i]<<", ";
        // std::cout<<intersection(graph[i], cycleContains)<<", ";
        assert(bitset_size(intersection(graph[i], cycleContains)) >= 2);
        count += bitset_size(intersection(graph[i], cycleContains));
    }
    assert(count%2==0);
    return count/2-cycle_length;
}

void printAndCountChords(bitset* graph, const int size_of_graph, const int* longestCycle, const int cycle_length){
		// countChords(graph, size_of_graph, longestCycle, cycle_length);
		std::cout<<"longest cycle: [";
		for (size_t i = 0; i < cycle_length-1; i++)
		{
			std::cout<<longestCycle[i]<<", ";
		}
		std::cout<<longestCycle[cycle_length-1]<<"]\n";
		std::cout<<"number of chords: " << countChords(graph, size_of_graph, longestCycle, cycle_length)<<std::endl;
		
}