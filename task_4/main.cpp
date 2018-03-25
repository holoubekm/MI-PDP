#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <vector>
#include <set>
#include <mpi.h>
#include <unistd.h>
#include <limits>

#include "worker.h"

#define MSG_NEED_TASKS 0x0
#define MSG_HAS_TASKS 0x1
#define MSG_RESULTS_AND_DIE 0x2

#define TAG_SEND_QUERY 		1000
#define TAG_SEND_NODES 		1001
#define TAG_SEND_GRAPH 		1002
#define TAG_SEND_TASKS 		1003
#define TAG_SEND_TASKS_CNT 	1004
#define TAG_SEND_RESULTS 	1005
#define TAG_SEND_LOCAL_BEST 	1006

using namespace std;

uint64_t maxDepth = 4;
uint64_t chunkSize = 500;

void sendGraph(G& graph, uint64_t subgraphSize, int worldSize) {
	uint64_t nodes = static_cast<uint64_t>(graph.size());
	uint8_t* data = new uint8_t[nodes * nodes];
	cout << "> Nodes " << nodes << endl;
	for(uint64_t x = 0; x < nodes; x++) {
		auto offset = x * nodes;
		for(uint64_t y = 0; y < nodes; y++) {
			data[offset + y] = graph[x][y];
		} 
	}
	for(int pid = 1; pid < worldSize; pid++) {
		MPI_Send(&nodes, 1, MPI_UNSIGNED_LONG_LONG, pid, TAG_SEND_NODES, MPI_COMM_WORLD);
		MPI_Send(&subgraphSize, 1, MPI_UNSIGNED_LONG_LONG, pid, TAG_SEND_NODES, MPI_COMM_WORLD);
		MPI_Send(data, nodes * nodes, MPI_BYTE, pid, TAG_SEND_GRAPH, MPI_COMM_WORLD);
	}
	delete[] data;
}

G receiveGraph(uint64_t& subgraphSize) {
	MPI_Status status;
	uint64_t nodes = 0;
	MPI_Recv(&nodes, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	MPI_Recv(&subgraphSize, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	uint8_t* data = new uint8_t[nodes * nodes];
	MPI_Recv(data, nodes * nodes, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	G graph;
	vector<bool> row;
	for(uint64_t x = 0; x < nodes; x++) {
		auto offset = x * nodes;
		for(uint64_t y = 0; y < nodes; y++) {
			row.push_back(data[offset + y]);
		}
		graph.push_back(row);
		row.clear();
	} 

	auto x = graph.size();
	auto y = 0;
	if(x != 0)
		y = graph[0].size();

	cout << "Received graph: " << x << " x " << y << endl;
	delete[] data;
	return graph;
}

void sendTasks(T& tasks, uint64_t globalBest, uint64_t maxDepth, uint64_t chunkSize, int pid) {
	uint8_t query = MSG_HAS_TASKS;
	MPI_Send(&query, 1, MPI_BYTE, pid, TAG_SEND_QUERY, MPI_COMM_WORLD);
	uint64_t tasksCnt = min(tasks.size(), chunkSize);
	MPI_Send(&tasksCnt, 1, MPI_UNSIGNED_LONG_LONG, pid, TAG_SEND_TASKS_CNT, MPI_COMM_WORLD);

	auto numbersCnt = 1 + tasksCnt * maxDepth;
	uint64_t* numbers = new uint64_t[numbersCnt];
	numbers[0] = globalBest;
	for(uint64_t x = 0; x < tasksCnt; x++) {
		auto& task = tasks.back();
		copy(task.begin(), task.end(), 1 + numbers + x * maxDepth);
		tasks.pop_back();
	}

	MPI_Send(numbers, numbersCnt, MPI_UNSIGNED_LONG_LONG, pid, TAG_SEND_TASKS, MPI_COMM_WORLD);
	delete[] numbers;
}

T receiveTasks(uint64_t& globalBest) {
	MPI_Status status;
	uint64_t tasksCnt = 0;
	MPI_Recv(&tasksCnt, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, TAG_SEND_TASKS_CNT, MPI_COMM_WORLD, &status);
	// cout << "< got " << tasksCnt << " tasksCnt" << endl;
	auto numbersCnt = 1 + tasksCnt * maxDepth;
	auto numbers = new uint64_t[numbersCnt];
	// cout << "< got " << numbersCnt << " numbers" << endl;
	MPI_Recv(numbers, numbersCnt, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, TAG_SEND_TASKS, MPI_COMM_WORLD, &status);
	// for(uint64_t x = 0; x < numbersCnt; x++)
		// cout << numbers[x] << " ";
	// cout << endl;
	globalBest = numbers[0];
	T tasks;
	for(uint64_t x = 0; x < tasksCnt; x++) {
		auto index = x * maxDepth;
		tasks.push_back(vector<uint64_t>(1 + numbers + index, 1 + numbers + index + maxDepth));
	}
	// cout << "prijato: " << tasks.size() << endl;
	delete[] numbers;
	return tasks;
}


R sendResultsAndDie(int pid, uint64_t resultsCnt) {
	MPI_Status status;
	uint8_t query = MSG_RESULTS_AND_DIE;
	MPI_Send(&query, 1, MPI_BYTE, pid, TAG_SEND_QUERY, MPI_COMM_WORLD);
	auto results = new uint64_t[resultsCnt];
	MPI_Recv(results, resultsCnt, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, TAG_SEND_RESULTS, MPI_COMM_WORLD, &status);
	vector<uint64_t> res(results, results + resultsCnt);
	delete[] results;
	return res;
}


int main(int argc, char* argv[]) {
	int rank, worldSize;
	uint8_t query;
	MPI_Status status;
	int provided = 0;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
	if(provided < MPI_THREAD_FUNNELED) {
		cout << "OpenMPI doesn't support the MPI_THREAD_FUNNELED threading model!" << endl;
		return 1;
	}
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char hostname[256];
	gethostname(hostname, sizeof(hostname));

	if(argc != 4) {
		cout << "./pdp input size threads" << endl;
		return 1;
	}

	auto threads = atoi(argv[3]);
	omp_set_num_threads(threads);

	if(rank == 0) {
		//Main process
		uint64_t globalBest = numeric_limits<uint64_t>::max();
		R globalResult;

		cout << "World size: " << worldSize << endl;
		cout << "Master " << getpid() << " at: " << hostname << endl; 
		auto worker = new Worker();
		auto g = worker->loadGraph(argc, argv);
		sendGraph(g, worker->mSubgraphSize, worldSize);
		auto tasks = worker->precomputeGlobalTasks(maxDepth);
		cout << "Tasks: " << tasks.size() << endl;
		cout << "NODES: " << worldSize << endl;
		cout << "THREADS: " << threads << endl;
		auto startTime = MPI_Wtime();
		while(!tasks.empty()) {
			cout << "> Tasks left: " << tasks.size() << endl;
			MPI_Recv(&query, 1, MPI_BYTE, MPI_ANY_SOURCE, TAG_SEND_QUERY, MPI_COMM_WORLD, &status);
			switch(query) {
				case MSG_NEED_TASKS:
					MPI_Recv(&query, 1, MPI_BYTE, MPI_ANY_SOURCE, TAG_SEND_LOCAL_BEST, MPI_COMM_WORLD, &status);
					globalBest = query < globalBest ? query : globalBest;
					sendTasks(tasks, globalBest, maxDepth, chunkSize, status.MPI_SOURCE);
					break;
				default:
					cout << "> UNKNOWN QUERY: " << query << endl;
					break;
			}
		}

		cout << "Going to kill all slaves :(" << endl;
		for(uint64_t x = 1; x < static_cast<uint64_t>(worldSize); x++) {
			MPI_Recv(&query, 1, MPI_BYTE, MPI_ANY_SOURCE, TAG_SEND_QUERY, MPI_COMM_WORLD, &status);
			switch(query) {
				case MSG_NEED_TASKS: {
					cout << " ----- " << status.MPI_SOURCE << " <TERMINATED>"<< endl;
					auto localResult = sendResultsAndDie(status.MPI_SOURCE, worker->mSubgraphSize);
					auto width = worker->evalResult(localResult);
					if(width <= globalBest) {
						globalBest = width;
						globalResult = localResult;
					} 
					break;
				}
				default:
					cout << "> UNKNOWN QUERY: " << query << endl;
					break;
			}
		}

		auto endTime = MPI_Wtime();
		cout << blue_start << "All slaves should have been finished :)" << blue_end << endl;
		cout << blue_start << "Best result: " << worker->evalResult(globalResult) << blue_end << endl;
		cout << blue_start << "Total elapsed time: " << (endTime - startTime) << "s" << blue_end << endl;
		cout << blue_start << "> Solution: ";
		for(uint64_t x = 0; x < globalResult.size(); x++)
			cout << globalResult[x] << " ";
		cout << blue_end << endl;
		delete worker;

	} else {
		//Slave processes
		cout << "Slave >" << rank << "< " << getpid() << " at: " << hostname << endl;
		uint64_t subgraphSize = 0;
		auto g = receiveGraph(subgraphSize);
		auto worker = new Worker();
		worker->setGraph(g, subgraphSize);

		bool running = true;
		while(running) {
			query = MSG_NEED_TASKS;
			MPI_Send(&query, 1, MPI_BYTE, 0, TAG_SEND_QUERY, MPI_COMM_WORLD);
			query = worker->getLocalBest();
			MPI_Send(&query, 1, MPI_BYTE, 0, TAG_SEND_LOCAL_BEST, MPI_COMM_WORLD);
			MPI_Recv(&query, 1, MPI_BYTE, 0, TAG_SEND_QUERY, MPI_COMM_WORLD, &status);
			switch(query) {
				case MSG_HAS_TASKS: {
					uint64_t globalBest = numeric_limits<uint64_t>::max();
					auto tasks = receiveTasks(globalBest);
					// for(uint64_t x = 0; x < subgraphSize; x++)
						// cout << tasks[0][x] << " ";
					// cout << endl;
					worker->setTasks(tasks);
					worker->setLocalBest(globalBest);
					worker->compute();
					break;
				}
				case MSG_RESULTS_AND_DIE: {
					auto results = worker->getResults();
					auto res = new uint64_t[results.size()];
					copy(results.begin(), results.end(), res);
					MPI_Send(res, results.size(), MPI_UNSIGNED_LONG_LONG, 0, TAG_SEND_RESULTS, MPI_COMM_WORLD);
					delete[] res;

					cout << red_start << "Kindly asked to die - " << rank << red_end << endl;
					running = false;
					break;
				}
				default: 
					cout << "Unknown query received" << endl;
					running = false;
					break;
			}
		}
		delete worker;

		cout << bred_start << "Process " << getpid() << " finished" << bred_end << endl;
	}
	MPI_Finalize();
	return 0;
}
