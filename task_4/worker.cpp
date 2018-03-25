#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <limits>
#include <iomanip>
#include <omp.h>
#include <chrono>
#include <thread>

#include "worker.h"

Worker::Worker() : mNodes(0), mSubgraphSize(0), mBestSol(numeric_limits<uint64_t>::max()) {

}

Worker::~Worker() {

}

void printGraph(G& graph) {
	auto nodes = graph.size();
	for (uint64_t i = 0; i < nodes; i++) {
		for (uint64_t j = 0; j < nodes; j++) {
			cout << graph[i][j];
		} cout << endl;
	} cout << endl;
}

G Worker::loadGraph(int argc, char* argv[]) {
	fstream input(argv[1]);
	if(!input.is_open()) {
		cerr << "can't open the file: " << argv[1];
		return G();
	} 

	istringstream is(argv[2]);
	is >> mSubgraphSize;
	cout << "Subgraph Size: " << mSubgraphSize << endl;

	input >> mNodes;
	vector<bool> line;
	while (input.get() != '\n');
	for (uint64_t i = 0; i < mNodes; i++){	
		for (uint64_t j = 0; j < mNodes; j++)
			line.push_back(input.get() != '0');
		while (input.get() != '\n');
		mGraph.push_back(line);
		line.clear();
	}

	printGraph(mGraph);
	computeEdges();
	return mGraph;
}

void Worker::precomputeTasks(uint64_t startN, uint64_t curN, uint64_t startK, uint64_t curK, uint64_t maxDepth, uint64_t* comb) {
	for(uint64_t i = curN; i <= startN - startK + curK; i++) {
		comb[curK] = i;
		if(curK == maxDepth - 1) {
#ifdef DUMP_TASKS
			for(uint64_t x = 0; x < maxDepth; x++)
				cout << comb[x] << " ";
			cout << endl;
#endif
			mTasks.push_back(vector<uint64_t>(comb, comb + maxDepth));
		} else {
			precomputeTasks(startN, i + 1, startK, curK + 1, maxDepth, comb);
		}
	}
}

void Worker::precomputeStarts(uint64_t startN, uint64_t curN, uint64_t startK, uint64_t curK, uint64_t maxDepth, uint64_t* comb) {
	for(uint64_t i = curN; i <= startN - startK + curK; i++) {
		comb[curK] = i;
		if(curK == maxDepth - 1) {
#ifdef DUMP_TASKS
			for(uint64_t x = 0; x < maxDepth; x++)
				cout << comb[x] << " ";
			cout << endl;
#endif
			mStarts.push_back(vector<uint64_t>(comb, comb + maxDepth));
		} else {
			precomputeStarts(startN, i + 1, startK, curK + 1, maxDepth, comb);
		}
	}
}

T Worker::precomputeGlobalTasks(uint64_t maxDepth) {
	if(mNodes == 0)
		return T();

	uint64_t* comb = new uint64_t[maxDepth];
	for(uint64_t x = 0; x < maxDepth; x++)
		comb[x] = x;
	precomputeTasks(mNodes, 0, mSubgraphSize, 0, maxDepth, comb);
	delete[] comb;
	return mTasks;
}

void Worker::setGraph(G& graph, uint64_t subgraphSize) {
	mGraph = graph;
	mNodes = graph.size();
	mSubgraphSize = subgraphSize;
	computeEdges();
}

void Worker::computeEdges() {
	for (uint64_t row = 0; row < mNodes; row++)
		for (uint64_t col = row + 1; col < mNodes; col++)
			if(mGraph[row][col])
				mEdges.push_back(make_pair(row, col));
}

void Worker::setTasks(T& tasks) {
	mTasks = tasks;
}

uint64_t Worker::getWidth(bool* selected) {
	uint64_t width = 0;
	for(auto& e : mEdges) {
		if(selected[e.first] != selected[e.second])
			width++;
		if(width >= mBestSol)
			return numeric_limits<uint64_t>::max();
	}
	return width;
}

uint64_t Worker::getBound(bool* selected, uint64_t i) {
	uint64_t width = 0;
	for(auto& e : mEdges) {
		if(e.first <= i && e.second <= i && selected[e.first] != selected[e.second])
			width++;
		if(width >= mBestSol)
			return numeric_limits<uint64_t>::max();
	}
	return width;
}

void Worker::BBDFSRec(uint64_t startN, uint64_t curN, uint64_t startK, uint64_t curK, uint64_t* arr, bool* selected) {
	for(uint64_t i = curN; i <= startN - startK + curK; i++) {
		arr[curK] = i;
		selected[i] = true;

		auto lower = getBound(selected, i);
		if(lower < mBestSol) {
			if(curK == startK - 1) {
	#ifdef DUMP_COMBS
				for(uint64_t x = 0; x < startK; x++)
					cout << setfill('0') << setw(2) << arr[x] << " ";
				cout << endl;
	#endif
				auto width = getWidth(selected);
				if(width < mBestSol) {
				#pragma omp critical
					if(width < mBestSol) {
						mBestSol = width;
						cout << green_start << "[+] Local Best: " << mBestSol << green_end << endl;
						mResults = R(arr, arr + startK);
					}
				}
			} else {
				BBDFSRec(startN, i + 1, startK, curK + 1, arr, selected);
			}
		}
		selected[i] = false;
	}
}

void Worker::compute() {
	if(mTasks.empty())
		return;
	auto cnt = mTasks.size();
	#pragma omp parallel for default(shared) schedule(static, 1)
	for(uint x = 0; x < cnt; x++) {
		// std::this_thread::sleep_for(5000us);
		auto& task = mTasks[x];
		auto taskDepth = task.size();

		auto localComb = new uint64_t[mSubgraphSize];
		auto localSelected = new bool[mNodes];
		memset(localComb, 0x0, sizeof(uint64_t) * mSubgraphSize);
		memset(localSelected, 0x0, sizeof(bool) * mNodes);
		
		for(uint64_t y = 0; y < taskDepth; y++) {
			localComb[y] = task[y];
			localSelected[task[y]] = true;
		}

		BBDFSRec(mNodes, localComb[taskDepth - 1] + 1, mSubgraphSize, taskDepth, localComb, localSelected);
		delete[] localComb;
		delete[] localSelected;
	}
}

R Worker::getResults() {
	if(mResults.size() != mSubgraphSize)
		for(uint64_t x = 0; x < mSubgraphSize; x++)
			mResults.push_back(x);
	return mResults;
}

uint64_t Worker::evalResult(R& result) {
	auto selected = new bool[mNodes];
	memset(selected, 0x0, sizeof(bool) * mNodes);
	for(auto res : result) {
		selected[res] = true;
	}
	auto width = getWidth(selected);
	delete[] selected;
	return width;
}

void Worker::setLocalBest(uint64_t localBest) {
	#pragma omp critical
	mBestSol = localBest;
}

uint64_t Worker::getLocalBest() {
	return mBestSol;
}