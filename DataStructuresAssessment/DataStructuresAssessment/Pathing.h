#pragma once

#include <vector>

struct Node {
public:
	Node(int x, int y);

	// neighbours: top(0), left(1), bottom(2), right(3)
	Node* neighbours[4];

	// if node can be traversed or not
	bool traversable;
	// position of the node on the graph
	int x, y;
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
};

/*
 * Pathfinds with a specific search, using the map.
 * Returns the path from start to end.
 * Check return values 'success' boolean before using path.
 */
NodeSearchResult pathfind(NodeSearch& search, Nodemap& map, Node* start, Node* end);