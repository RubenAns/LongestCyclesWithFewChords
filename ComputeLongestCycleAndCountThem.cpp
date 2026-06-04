#include <iostream>
#include <cstring>
#include "bitset64.hpp"
#include <cassert>
using namespace std;
#define maxVertices 64


static int currentCycle[maxVertices]; 
static int currentCycleSize;
static int longestCycle[maxVertices];
static int longestCycleSize = 0;
static int numberOfCycles = 0;

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
void initializeLongestCycle(bitset graph[], const int n);



int computeLongestCycleAndCountThem(bitset* graph, const int n, int* longestCycleResult, int& length) {
	containsSet = EMPTY;
	longestCycleSize = 0;
	for (int i = 0; i < n; i++)
	{
		smallestVertex = i;
		initializeLongestCycle(graph,n);
		if (n - (i + 1) < longestCycleSize) {
			break;
		}
	}
	std::copy(longestCycle, longestCycle + longestCycleSize, longestCycleResult);
	length = longestCycleSize;
	return numberOfCycles;
}
void initializeLongestCycle(bitset graph[], const int n){
	currentCycle[1] = smallestVertex;
	forEachAfterIndex(i, graph[smallestVertex], smallestVertex){
		forEachAfterIndex(j, graph[smallestVertex], i){
			recentVertex = i;
			lastVertex = j;
			currentCycle[0] = lastVertex;
			currentCycle[2] = recentVertex;
			add(containsSet,recentVertex);
			add(containsSet,smallestVertex);
			//!! do not add lastVertex, or we will never complete the cycle!!
			currentCycleSize = 3;
			computeLongestCycleRec(graph,n);
			removeElement(containsSet, j);
		}
		removeElement(containsSet, i);

	}
}

void computeLongestCycleRec(bitset graph[], const int n){
	//loop over all possible nodes connected by an edge but not yet contained
	forEachAfterIndex(i, difference(graph[recentVertex], containsSet ), smallestVertex) //containsSet[lastVertex]=0 always!
	{	
		assert(((1<<i) & graph[recentVertex] )!= 0);
		if (i == lastVertex) 
			{
				//cycle completed
				if (currentCycleSize > longestCycleSize) {
					std::copy(currentCycle, currentCycle + currentCycleSize, longestCycle);
					longestCycleSize = currentCycleSize;
					numberOfCycles = 0;
				}
				if (currentCycleSize == longestCycleSize){
					numberOfCycles++;
				}
			}
		else {
				//extend with selected vertex
				add(containsSet,i);
				currentCycle[currentCycleSize++]=i;
				recentVertex = i;
				computeLongestCycleRec(graph,n);
				removeElement(containsSet, i);
				recentVertex = currentCycle[currentCycleSize-2];
				currentCycleSize--;
			}
		}
	}
	


	
	

#ifdef TEST_HELPER
//This code is meant as a test of this class and does not add any functionality
int main() {
	//F?lv_
//  bitset graph[] = {14LL, 13LL, 11LL, 7LL};
	bitset graph[8];
    std::cout<<8<<std::endl;
	int n=8;
    loadGraphBitset("Gs@ipo", n, graph);
	// bitset graph[] = {80, 96, 112, 112, 13, 14, 15};
	int result[8];
	int cycleLength = computeLongestCycleWithFewestChords(graph,n,result);
	printArray(result, 8);
	std::cout << "Longest cycle: " << std::endl;
	std::cout << cycleLength << std::endl;
	std::cout << numberOfLongestCycles << std::endl;
	return 0;
}
#endif


