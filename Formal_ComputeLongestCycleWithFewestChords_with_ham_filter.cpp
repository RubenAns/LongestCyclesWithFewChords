#include "ComputeLongestCycle_copy.hpp"
#include <iostream>
#include <cstring>
#include "bitset64.hpp"
#include <cassert>
#include "CountChords.hpp"
// #include "GraphAPI.hpp"
using namespace std;
#define maxVertices 64


static int currentCycle[maxVertices]; 
static int currentCycleSize;
static int longestCycle[maxVertices];
static int longestCycleSize = 0;
static int numberOfChords = 0;

static int lastVertex = -1;
static int recentVertex = -1;
static int smallestVertex = -1;
static bitset containsSet;


void printArray(const int* array, const int size) {
	std::cout<<"This is the array: ";
	for (int i =0; i<size; i++)	std::cout << array[i] << " ";
	std::cout<<std::endl;
}

void computeLongestCycleRec(bitset graph[], const int n);
void initializeLongestCycleAndFind(bitset graph[], const int n);

int computeLongestCycleWithFewestChordsWithHamFilter(bitset* graph, const int n, int* longestCycleResult) {
	containsSet = EMPTY;
	longestCycleSize = 0;
	for (int i = 0; i < n; i++)
	{
		/*Iterate from smallest to largest vertex: 
		start by trying to find a longest cycle containing vertex 0, then one containing vertex 1 but not 0, etc*/
		smallestVertex = i;
		initializeLongestCycleAndFind(graph,n);

		/*If we have found a cycle of length L and next iteration we will remove the first i+1 vertices from our searchspace,
		we can only find cycles of length n-(i+1)*/
		if (n - (i + 1) < longestCycleSize) {
			break;
		}
	}
	std::copy(longestCycle, longestCycle + longestCycleSize, longestCycleResult);
	return longestCycleSize;
}
void initializeLongestCycleAndFind(bitset graph[], const int n){
	currentCycle[1] = smallestVertex;
	forEachAfterIndex(i, graph[smallestVertex], smallestVertex){
		forEachAfterIndex(j, graph[smallestVertex], i){
			recentVertex = i;
			lastVertex = j;
			currentCycle[0] = lastVertex;
			currentCycle[2] = recentVertex;
			add(containsSet,recentVertex);
			add(containsSet,smallestVertex);
			/*We do not add the lastVertex to the containsSet, such that later, we could still detect if we can add this vertex and complete the cycle */
			
			currentCycleSize = 3;
			computeLongestCycleRec(graph,n);
			if (longestCycleSize == n) return; //hamiltionian cycle found
			removeElement(containsSet, j);
		}
		removeElement(containsSet, i);
	}
}

void computeLongestCycleRec(bitset graph[], const int n){
	if (longestCycleSize == n) return; //hamiltionian cycle found

	/*extend the path by any vertex 
	- connected to the recently added vertex (the end of the current path)
	- not yet in the path (or our last vertex) 
	- bigger in index than the starting vertex */
	forEachAfterIndex(i, difference(graph[recentVertex], containsSet ), smallestVertex) //reminder: containsSet[lastVertex]=0 always!
	{		
		if (i == lastVertex)  /*the cycle is complete*/
			{
				/*LONGER CYCLE FOUND*/
				if (currentCycleSize > longestCycleSize) {
					std::copy(currentCycle, currentCycle + currentCycleSize, longestCycle);
					longestCycleSize = currentCycleSize;
					numberOfChords = countChords(graph, n, longestCycle, longestCycleSize);

					if (currentCycleSize == n) {
						/*graph is hamiltonian, not interesting for us*/
						longestCycleSize = n;
						numberOfChords = 10e8; //big unrealistic number
						return;
					}
				}
				/*EQUALLY LONG CYCLE FOUND WITH POTENTIALLY LESS CHORDS*/
				if (currentCycleSize == longestCycleSize){
					int numberOfChords2 = countChords(graph, n, longestCycle, longestCycleSize);
					if (numberOfChords2 < numberOfChords){
						numberOfChords = numberOfChords2;
						std::copy(currentCycle, currentCycle + currentCycleSize, longestCycle);
					}
				}
			}
		else { /*Not a cycle, but extend the path and recurse*/
				add(containsSet,i);
				currentCycle[currentCycleSize++]=i;
				recentVertex = i;
				computeLongestCycleRec(graph,n);
				removeElement(containsSet, i);
				recentVertex = currentCycle[currentCycleSize-2];
				currentCycleSize--; //note that the last vertex in currentCycle is not deleted, but only the size is decreased
			}
		}
	}
	

