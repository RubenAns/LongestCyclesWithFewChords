#include "ComputeLongestCycleAndCountThem.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <cstring>

using namespace std;

static std::vector<int> currentCycle = {};
static std::vector<int> longestCylce;
static int longestCycleSize;
int numberOfCycles = 0;

static int lastVertex = -1;
static int firstVertex = -1;
static std::vector<int> contains;


template <typename S>
ostream& operator<<(ostream& os,
	const vector<S>& vector) {

	// Printing all the elements using <<
	for (auto i : vector)
		os << i << " ";
	return os;
}

void computeLongestCycleRec(std::vector< std::vector<int>>& graph, const int n); 

int computeLongestCycleMatrix(std::vector< std::vector<int>>& graph, std::vector<int>& longestCycleReturn) {
	const int n = graph.size();
	contains.assign(n,0);
	longestCycleSize = 0;
	for (int i = 0; i < n; i++)
	{
		lastVertex = i;
		firstVertex = i;
		currentCycle = { i };
		computeLongestCycleRec(graph, n);
		if (n - (i + 1) < longestCycleSize - 1) {
			break;
		}
	}
	longestCycleReturn = longestCylce;
	return numberOfCycles;
}

void computeLongestCycleRec(std::vector< std::vector<int>>& graph, const int n){

 	for (int i = firstVertex; i < n; i++)
	{

		if (graph[lastVertex][i] == 1 && contains[i] == 0) //contains[firstvertex]=0 always!
		{
			if (i == firstVertex)
			{
				//cycle completed
				if ((int) currentCycle.size() > longestCycleSize && currentCycle.size()>2) {
					//update longest
					longestCylce = currentCycle;
					longestCylce.push_back(i);
					longestCycleSize = longestCylce.size()-1;
					numberOfCycles = 0;
				}
				if ((int) currentCycle.size() == longestCycleSize) numberOfCycles++;
			}
			else {
				//extend with selected vertex
				contains[i] = 1;
				currentCycle.push_back(i);
				lastVertex = i;
				computeLongestCycleRec(graph, n);
				contains[i] = 0;
				currentCycle.pop_back();
				lastVertex = currentCycle.back();
			}
		}
	}	
}

#ifdef TEST_HELPER
//This code is meant as a test of this class and does not add any functionality
int main() {
vector<vector<int>> graph = {{0, 1, 1, 1, 1},
		{1, 0, 1, 1,1},
		{1, 1, 0, 1,1},
		{1, 1, 1, 0,1},
		{1, 1, 1, 1, 0}};

	auto cylce = computeLongestCycleMatrix(graph);
	std::cout << "Longest cycle: " << cylce.size()-1 << std::endl;
	std::cout << cylce << std::endl;
	return 0;
}
#endif


