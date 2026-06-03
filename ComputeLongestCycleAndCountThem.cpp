#include <iostream>
#include <cstring>
#include "bitset64.hpp"
#include <cassert>
using namespace std;
#define maxVertices 64


static int currentCycle[maxVertices]; //gebruik pointers en arrays, zie via profiler met const max_int
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

void computeLongestCycleRec(bitset graph[], const int n); // Add this function declaration before its first use
void initializeLongestCycle(bitset graph[], const int n);



int computeLongestCycleAndCountThem(bitset* graph, const int n, int* longestCycleResult, int& length) {
	// std::cout<<"input = " << graph[0]<<", "<< graph[1] <<", n= "<<n<<std::endl;
	containsSet = EMPTY;
	longestCycleSize = 0;
	for (int i = 0; i < n; i++)
	{
		smallestVertex = i;
		initializeLongestCycle(graph,n);
		// std::cout << "iteratie number: " << i << std::endl;
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
			// std::cout << "i: " << recentVertex << ", j: " << lastVertex << std::endl;
			currentCycle[0] = lastVertex;
			currentCycle[2] = recentVertex;
			// printArray(currentCycle,4);
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
		// std::cout << "now trying node " << i << " with current size = " << currentCycleSize << " with last vertex = " << lastVertex<< std::endl;
		// printArray(currentCycle,currentCycleSize);		
		assert(((1<<i) & graph[recentVertex] )!= 0);
		if (i == lastVertex) 
			{
				//cycle completed
				if (currentCycleSize > longestCycleSize) {
					std::copy(currentCycle, currentCycle + currentCycleSize, longestCycle);
					longestCycleSize = currentCycleSize;
					numberOfCycles = 0;

					// printArray(longestCycle, longestCycleSize);
					// std::cout << "longesCycleSize = " << longestCycleSize << std::endl;
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
				//deletethis
				currentCycle[currentCycleSize-1] = 0;
				recentVertex = currentCycle[currentCycleSize-2];
				currentCycleSize--;
			}
		}
	}
	


	
	

#ifdef TEST_HELPER
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


