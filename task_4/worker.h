#include <vector>
#include <cstdint>

using namespace std;

#define red_start			"\033[10;91m" 
#define red_end			"\033[0m"

#define bred_start			"\033[1;91m" 
#define bred_end			"\033[0m"

#define green_start			"\033[10;92m" 
#define green_end			"\033[0m"

#define blue_start			"\033[10;94m" 
#define blue_end			"\033[0m"


typedef vector<vector<bool>> G; //Graph
typedef vector<pair<uint64_t, uint64_t>> E; // Edges
typedef vector<uint64_t> S; //Starts for the local combinations
typedef vector<S> T; //Tasks -> sequence of combination numbers, length = maxDepth
typedef vector<uint64_t> R; //Result

class Worker {
public:
	Worker();
	~Worker();
	G loadGraph(int argc, char* argv[]);
	T precomputeGlobalTasks(uint64_t maxDepth);
	void setGraph(G& graph, uint64_t subgraphSize);
	void setTasks(T& tasks);
	void setLocalBest(uint64_t localBest);
	uint64_t getLocalBest();
	void compute();
	R getResults();
	uint64_t evalResult(R& result);

private:
	void computeEdges();
	void precomputeTasks(uint64_t startN, uint64_t curN, uint64_t startK, uint64_t curK, uint64_t maxDepth, uint64_t* comb);
	void precomputeStarts(uint64_t startN, uint64_t curN, uint64_t startK, uint64_t curK, uint64_t maxDepth, uint64_t* comb);
	uint64_t getWidth(bool* selected);
	uint64_t getBound(bool* selected, uint64_t i);
	void BBDFSRec(uint64_t startN, uint64_t curN, uint64_t startK, uint64_t curK, uint64_t* arr, bool* selected);


// private:
public:
	uint64_t mNodes;
	uint64_t mSubgraphSize;
	G mGraph;
	E mEdges;
	T mTasks;
	T mStarts;

	uint64_t mBestSol;
	R mResults;
};

