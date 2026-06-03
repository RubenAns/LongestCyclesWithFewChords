#include "ReadGraph.hpp"
#include <vector>
#include <string>
#include <iostream>

#define unsafePrev(character, current) (__builtin_ctz(character) - current >= 0 ? -1 : current -__builtin_clz((character) << (32 - current)) - 1)

#define prev(character,current) (character ? unsafePrev(character,current) : -1)

using namespace std;

const int nb_bits=4096;
#define MAXVERTICES 4096

int vertexQ[MAXVERTICES];
int qStart;
int qFinish;

int distBFS1[MAXVERTICES];
int numberShortestPaths[MAXVERTICES];

static vector< vector<int> > graph;

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<int>>& matrix) {
    os << "[\n";
    for (const auto& row : matrix) {
        os << "  [ ";
        for (const auto& val : row) {
            os << val << " ";
        }
        os << "]\n";
    }
    os << "]";
    return os;
}



void loadGraphMatrix(string graphString, int numberOfVertices, vector<vector<int>>& graph) {
    graph.assign(numberOfVertices, std::vector<int>(numberOfVertices, 0));

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
		printf("Error: Program can only handle graphs with %d vertices or fewer.\n",MAXVERTICES);
		abort();
	}

	int currentVertex = 1;
	int sum = 0; 
 	for (int index = startIndex; index<(int)graphString.size(); index++) {
		int i;
		for (i = prev(graphString[index] - 63, 6); i != -1; i = prev(graphString[index] - 63, i)) {
			while(5-i+(index-startIndex)*6 - sum >= 0) {
				sum += currentVertex;
				currentVertex++;
			}
			sum -= --currentVertex;
			int neighbour = 5-i+(index - startIndex)*6 - sum;
            graph[currentVertex][neighbour] = 1;
            graph[neighbour][currentVertex] = 1;
		}
	}
}

#ifdef TEST_HELPER
int main()
{
    cout << "input your graph:" << endl;
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long nb_graphs_read_from_input=0;
    string line;
    while(getline(cin,line))
    {
        //line+="\n";
        nb_graphs_read_from_input++;
        int n = getNumberOfVertices(line);
        loadGraphMatrix(line, n, graph);
        cout << graph << endl;
    }
    return 0;
}
#endif