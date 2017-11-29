#pragma once

#include <vector>
#include <map>
#include <functional>
#include <queue>

struct Node {
public:
	Node(int x, int y);

	// neighbours: top(0), left(1), bottom(2), right(3)
	Node* neighbours[4];

	// if node can be traversed or not
	bool traversable;

	// position of the node on the graph
	int x, y;

	// weight of the node, used for weighted searches like A*
	// default weight value is 1
	int weight;
};

class Nodemap {
private:
	struct NodeContainer {
		std::vector<Node*> _nodes;
	};

	// width & height of the nodemap
	int _width, _height;
	// forms a dynamic 2d array, height->width
	std::vector<NodeContainer> _containers;

public:
	Nodemap(int width, int height);

	Node* getNode(int x, int y);

	int getWidth();

	int getHeight();
};

/*
 * Result of the node search.
 */
struct NodeSearchResult {
public:
	/*
	 * The path result from whatever search was used.
	 */
	std::vector<Node*> path;

	/*
	 * All nodes that were traversed in the search.
	 */
	std::vector<Node*> traversed;

	/*
	 * If the search was successful or not.
	 */
	bool success; 

	/*
	 * The time that it took to perform the search. 
	 */
	double time;
};

/*
 * Heuristic calculation between A and B.
 */
int heuristic(Node* a, Node* b);

/*
 * Wrapper around std::priority_queue that allows 
 * prioritization of elements through a given value
 * rather than a comparator between elements.
 */
template<typename Value, typename PriorityValue = int>
class PriorityQueue {
private:
	// pair where integer represents the priority
	// and the T value represents our queue type
	typedef std::pair<PriorityValue, Value> qPair;

	// the standard lib priority queue that we're operating on 
	std::priority_queue<qPair, std::vector<qPair>, std::greater<qPair>> _pQueue;

public:
	/*
	 * If our queue is empty or not.
	 */
	inline bool empty() {
		return _pQueue.empty();
	}

	/* 
	 * Put an item into the queue with a given priority.
	 */
	inline void put(Value item, PriorityValue priority) {
		_pQueue.emplace(priority, item);
	}

	/*
	 * Gets the top value of the priority queue - and also pops it.
	 */
	inline Value get() {
		Value top = _pQueue.top().second;
		_pQueue.pop();
		return top;
	}
};

/*
* Abstract base class of a search.
*/
class NodeSearch {
public:
	virtual NodeSearchResult search(Nodemap& nodemap, Node* start, Node* end) = 0;
};

class BreadthFirstSearch : public NodeSearch {
public:
	NodeSearchResult search(Nodemap& nodemap, Node* start, Node* end) override;
	void buildPath(NodeSearchResult &result, Node * front, std::map<Node *, Node *> &cameFrom, std::map<Node *, bool> &visited);
};

class AStarSearch : public NodeSearch {
public:
	NodeSearchResult search(Nodemap& nodemap, Node* start, Node* end) override;
	void buildPath(NodeSearchResult &result, Node * front, std::map<Node *, Node *> &cameFrom, std::map<Node *, int> &costSoFar);
};

/*
 * Pathfinds with a specific search, using the map.
 * Returns the path from start to end.
 * Check return values 'success' boolean before using path.
 */
NodeSearchResult pathfind(NodeSearch& search, Nodemap& map, Node* start, Node* end);