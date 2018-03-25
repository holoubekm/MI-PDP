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
// #include <unistd.h>
#include <cstring>

using namespace std;

typedef vector<vector<bool>> G;
typedef vector<pair<uint64_t, uint64_t>> E;
typedef set<uint64_t> S;

uint64_t bestSol = numeric_limits<uint64_t>::max();
E edges;

#ifndef THREADS
#define THREADS 1
#endif

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

void BBDFSRec(G& graph, uint64_t startN, uint64_t curN, uint64_t startK, uint64_t curK, uint64_t* arr, bool* selected) {
	for(uint64_t i = curN; i <= startN - startK + curK; i++) {
		arr[curK] = i;
		selected[i] = true;

		auto lower = getBound(selected, i);
		if(lower < bestSol) {
			if(curK == startK - 1) {
				auto width = getWidth(selected);
				if(width < bestSol)
				#pragma omp critical
				{
					if(width < bestSol)
						bestSol = width;
				}
			} else {
				BBDFSRec(graph, startN, i + 1, startK, curK + 1, arr, selected);
			}
		}
		selected[i] = false;
	}
}

vector<vector<uint64_t>> starts;
void precomputeStarts(uint64_t startN, uint64_t curN, uint64_t startK, uint64_t curK, uint64_t maxDepth, uint64_t* comb) {
	for(uint64_t i = curN; i <= startN - startK + curK; i++) {
		comb[curK] = i;
		if(curK == maxDepth - 1) {
			starts.push_back(vector<uint64_t>(comb, comb + maxDepth));
		} else {
			precomputeStarts(startN, i + 1, startK, curK + 1, maxDepth, comb);
		}
	}
}

void BBDFS(G& graph, uint64_t nodes, uint64_t subgraph_size) {
	uint64_t* precomputed = new uint64_t[subgraph_size];
	for(uint64_t x = 0; x < subgraph_size; x++)
		precomputed[x] = x;

	uint64_t maxDepth = 3;
	precomputeStarts(nodes, 0, subgraph_size, 0, maxDepth, precomputed);
	delete[] precomputed;

	for (uint64_t row = 0; row < nodes; row++)
		for (uint64_t col = row + 1; col < nodes; col++)
			if(graph[row][col])
				edges.push_back(make_pair(row, col));

	auto count = starts.size();
	omp_set_num_threads(THREADS);
	#pragma omp parallel for default(shared) schedule(static, 1)
	for(uint64_t x = 0; x < count; x++) {
		auto start = starts[x];
		auto localComb = new uint64_t[subgraph_size];
		auto localSelected = new bool[nodes];
		memset(localComb, 0x0, sizeof(uint64_t) * subgraph_size);
		memset(localSelected, 0x0, sizeof(bool) * nodes);
		for(uint64_t x = 0; x < maxDepth; x++) {
			localComb[x] = start[x];
			localSelected[start[x]] = true;
		}
		//Repair the rest of the combination
		for(uint64_t x = maxDepth, val = localComb[x - 1] + 1; x < subgraph_size; x++, val++)
			localComb[x] = val;

		BBDFSRec(graph, nodes, localComb[maxDepth - 1] + 1, subgraph_size, maxDepth, localComb, localSelected);
		delete[] localComb;
		delete[] localSelected;
	}
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
	cout << "Threads: " << THREADS << endl;

	const auto wtime_start = omp_get_wtime();
	BBDFS(graph, nodes, subgraph_size);
	cout << "result width: " << bestSol << endl;
	const auto wtime_end = omp_get_wtime();
	cout << "total time: " << (wtime_end - wtime_start) << endl;
	return 0;
}
