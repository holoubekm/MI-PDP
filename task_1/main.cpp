#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <vector>
#include <sstream>
#include <set>
#include <limits>
#include <iomanip>
#include <omp.h>
#include <cstring>

using namespace std;

typedef vector<vector<bool>> G;
typedef vector<pair<uint64_t, uint64_t>> E;
typedef set<uint64_t> S;

uint64_t bestSol = numeric_limits<uint64_t>::max();
E edges;

uint64_t loadGraph(G& graph, fstream& input) {
	uint64_t nodes;
	input >> nodes;

	vector<bool> line;
	while (input.get() != '\n');
	for (uint64_t i = 0; i < nodes; i++){	
		for (uint64_t j = 0; j < nodes; j++)
			line.push_back(input.get() != '0');
		while (input.get() != '\n');
		graph.push_back(line);
		line.clear();
	}
	return nodes;
}

void printGraph(G& graph) {
	auto nodes = graph.size();
	for (uint64_t i = 0; i < nodes; i++) {
		for (uint64_t j = 0; j < nodes; j++) {
			cout << graph[i][j];
		} cout << endl;
	} cout << endl;
}


uint64_t getWidth(bool* selected) {
	uint64_t width = 0;
	for(auto& e : edges) {
		if(selected[e.first] != selected[e.second])
			width++;
		if(width >= bestSol)
			return numeric_limits<uint64_t>::max();
	}
	return width;
}

uint64_t getBound(bool* selected, uint64_t i) {
	uint64_t width = 0;
	for(auto& e : edges) {
		if(e.first <= i && e.second <= i && selected[e.first] != selected[e.second])
			width++;
		if(width >= bestSol)
			return numeric_limits<uint64_t>::max();
	}
	return width;
}

void BBDFSRec(G& graph, uint64_t startN, uint64_t curN, uint64_t startK, uint64_t curK, uint64_t* arr, bool* selected, int depth) {
	for(uint64_t i = curN; i <= startN - startK + curK; i++) {
		arr[curK] = i;
		selected[i] = true;

		auto lower = 0;
		if(depth >= 4)
			lower = getBound(selected, i);
		if(lower < bestSol) {
			if(curK == startK - 1) {
				auto width = getWidth(selected);
				if(width < bestSol)
					bestSol = width;
			} else {
				BBDFSRec(graph, startN, i + 1, startK, curK + 1, arr, selected, depth + 1);
			}
		}
		selected[i] = false;
	}
}

void BBDFS(G& graph, uint64_t nodes, uint64_t subgraph_size) {

	for (uint64_t row = 0; row < nodes; row++)
		for (uint64_t col = row + 1; col < nodes; col++)
			if(graph[row][col])
				edges.push_back(make_pair(row, col));


	auto localComb = new uint64_t[subgraph_size];
	auto localSelected = new bool[nodes];
	memset(localComb, 0x0, sizeof(uint64_t) * subgraph_size);
	memset(localSelected, 0x0, sizeof(bool) * nodes);

	BBDFSRec(graph, nodes, 0, subgraph_size, 0, localComb, localSelected, 0);
	delete[] localComb;
	delete[] localSelected;
}

int main(int argc, char* argv[]) {

	if(argc != 3)
		cout << "./pdp input size" << endl;

	fstream input(argv[1]);
	if(!input.is_open()) {
		cerr << "can't open the file: " << argv[2];
		return 1;
	} 

	istringstream is(argv[2]);
	uint64_t subgraph_size = 0;
	is >> subgraph_size;

	G graph;
	auto nodes = loadGraph(graph, input);
	input.close();

	printGraph(graph);

	cout << "Graph size: " << nodes << ", subgraph_size: " << subgraph_size << endl;

	const auto wtime_start = omp_get_wtime();
	BBDFS(graph, nodes, subgraph_size);
	cout << "result width: " << bestSol << endl;
	const auto wtime_end = omp_get_wtime();
	cout << "total time: " << (wtime_end - wtime_start) << endl;
	return 0;
}
