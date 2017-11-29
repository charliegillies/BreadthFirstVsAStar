#include "Pathing.h"

#include <math.h>
#include <chrono>
using namespace std::chrono;


Node::Node(int x, int y) : traversable(true), x(x), y(y), weight(1) { }

Nodemap::Nodemap(int width, int height) : _width(width), _height(height)
{
	// setup & populate the 2d array
	for (int i = 0; i < height; i++) {
		NodeContainer container = NodeContainer();
		for (int j = 0; j < width; j++) {
			container._nodes.push_back(new Node(j, i));
		}
		_containers.push_back(container);
	}

	// now assign node neighbours
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Node* current = getNode(x, y);

			current->neighbours[0] = (y > 0)			? getNode(x, y - 1) : nullptr;
			current->neighbours[1] = (x > 0)			? getNode(x - 1, y) : nullptr;
			current->neighbours[2] = (y < (height - 1)) ? getNode(x, y + 1) : nullptr;
			current->neighbours[3] = (x < (width - 1))	? getNode(x + 1, y) : nullptr;
		}
	}
}

Node * Nodemap::getNode(int x, int y)
{
	return _containers[y]._nodes[x];
}

int Nodemap::getWidth()
{
	return _width;
}

int Nodemap::getHeight()
{
	return _height;
}

int heuristic(Node * a, Node * b)
{
	return abs(a->x - b->x) + abs(a->y + b->y);
}

NodeSearchResult pathfind(NodeSearch & search, Nodemap & map, Node * start, Node * end)
{
	// begin timer...
	auto start_clock = high_resolution_clock::now();

	// begin the search
	NodeSearchResult result = search.search(map, start, end);
	
	// end timer..
	auto end_clock = high_resolution_clock::now();

	// set time
	duration<double> timespan = duration_cast<duration<double>>(end_clock - start_clock);
	result.time = timespan.count();

	return result;
}

NodeSearchResult BreadthFirstSearch::search(Nodemap & nodemap, Node * start, Node * end)
{
	NodeSearchResult result;

	// early exit if start or end cannot be traversed
	if (!start->traversable || !end->traversable) {
		result.success = false;
		return result;
	}

	// the expanding ring, starts at the start node
	std::queue<Node*> frontier;
	frontier.push(start);

	// map indicating what nodes have been visited
	std::map<Node*, bool> visited;

	// map indicating how the traversal happened
	std::map<Node*, Node*> cameFrom;
	cameFrom[start] = nullptr;
 
	while (!frontier.empty()) {
		// get front element in the frontier, remove it from the queue
		Node* front = frontier.front();
		frontier.pop();

		// we've reached the point we want to be at..
		if (front == end) {
			result.success = true;

			// build path..
			Node* pnode = front;
			while (pnode != nullptr) {
				result.path.push_back(pnode);
				pnode = cameFrom[pnode];
			}

			for (auto visited_pair : visited)
				if (visited_pair.second)
					result.traversed.push_back(visited_pair.first);

			std::reverse(result.path.begin(), result.path.end());
			return result;
		}

		// mark as visited
		visited[front] = true;

		// check neighbours for frontier eligibility
		for (int i = 0; i < 4; i++) {
			// neighbour
			Node* neighbour = front->neighbours[i];

			// if neighbour doesn't exist, or has been visited already..
			if (neighbour == nullptr || visited[neighbour] || !neighbour->traversable) continue;

			// mark visited
			visited[neighbour] = true;

			// we came from the current top
			cameFrom[neighbour] = front;

			// put into frontier for checking
			frontier.push(neighbour);
		}

	}

	// if we've reached here, we failed.
	result.success = false;
	return result;
}

NodeSearchResult AStarSearch::search(Nodemap & nodemap, Node * start, Node * end)
{
	// A* Search..
	NodeSearchResult result;

	// early exit if start or end cannot be traversed
	if (!start->traversable || !end->traversable) {
		result.success = false;
		return result;
	}

	// the expanding ring, starts at the start node
	PriorityQueue<Node*> frontier;
	frontier.put(start, 0);

	// map indicating how the traversal happened
	std::map<Node*, Node*> cameFrom;
	cameFrom[start] = nullptr;

	// the calculated heuristics
	std::map<Node*, int> costSoFar;
	costSoFar[start] = 0;

	while (!frontier.empty()) {
		Node* front = frontier.get();

		// early exit..
		if (front == end) {
			result.success = true;

			// build path..
			Node* pnode = front;
			while (pnode != nullptr) {
				result.path.push_back(pnode);
				pnode = cameFrom[pnode];
			}

			for (auto visited_pair : costSoFar)
				result.traversed.push_back(visited_pair.first);

			std::reverse(result.path.begin(), result.path.end());
			return result;
		}

		// check neighbours for frontier eligibility
		for (Node* next : front->neighbours) {

			// if neighbour doesn't exist, or is not traversable.. go to next
			if (next == nullptr || !next->traversable) continue;

			// current cost of our traversal
			int currentCost = costSoFar[front];

			// the potential new cost of the movement
			int newcost = currentCost + next->weight;

			// if the cost so far doesn't have our neighbour
			// or.. the new cost is better than our current cost
			if (costSoFar.find(next) == costSoFar.end() || newcost < currentCost) {
				// set the cost so far
				costSoFar[next] = currentCost;

				// we came from the current top
				cameFrom[next] = front;

				// calculate priority & put it into the frontier
				int priority = newcost + heuristic(end, next);
				frontier.put(next, newcost);
			}
		}
	}


	// Default return case
	result.success = false;
	return result;
}
