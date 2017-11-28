#include "Pathing.h"

#include <queue>
#include <map>

Node::Node(int x, int y) : traversable(false), x(x), y(y) { }

Nodemap::Nodemap(int width, int height)
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

NodeSearchResult pathfind(NodeSearch & search, Nodemap & map, Node * start, Node * end)
{
	NodeSearchResult result = search.search(map, start, end);
	return result;
}

NodeSearchResult BreadthFirstSearch::search(Nodemap & nodemap, Node * start, Node * end)
{
	// the expanding ring, starts at start
	std::queue<Node*> frontier;
	frontier.push(start);

	// map indicating what nodes have been visited
	std::map<Node*, bool> visited;

	// map indicating how the traveral happened
	std::map<Node*, Node*> cameFrom;
	cameFrom[start] = nullptr;
 
	while (!frontier.empty()) {
		// get front element in the frontier, remove it from the queue
		Node* front = frontier.front();
		frontier.pop();

		// we've reached the point we want to be at..
		if (front == end) {
			NodeSearchResult result;
			result.success = true;

			// build path..
			Node* pnode = front;
			while (pnode != nullptr) {
				result.path.push_back(pnode);
				pnode = cameFrom[pnode];
			}

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
			if (neighbour == nullptr || visited[neighbour]) continue;

			// mark visited
			visited[neighbour] = true;

			// we came from the current top
			cameFrom[neighbour] = front;

			// put into frontier for checking
			frontier.push(neighbour);
		}

	}

	// if we've reached here, we failed.
	NodeSearchResult result;
	result.success = false;
	return result;
}
