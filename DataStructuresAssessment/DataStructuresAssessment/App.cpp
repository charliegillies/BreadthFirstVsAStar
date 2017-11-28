#include "App.h"
#include <allegro5\allegro_primitives.h>

App::App() { }

void App::initialize()
{
	// build a nodemap
	_nodeMap = new Nodemap(30, 18);

	_nodeMap->getNode(8, 8)->traversable = false;
	_nodeMap->getNode(0, 2)->traversable = false;

	_start = _nodeMap->getNode(0, 0);
	_end = _nodeMap->getNode(3, 3);
	
	_searchResult = pathfind(BreadthFirstSearch(), *_nodeMap, _start, _end);
}

void App::update(float dt) { }

bool node_position_in_list(int x, int y, std::vector<Node*>& nodeList) {
	// check for every node in the list..
	for (auto node : nodeList) {
		if (node->x == x && node->y == y)
			return true;
	}

	return false;
}

void App::render()
{
	static const int size = 32;
	static ALLEGRO_COLOR gray	= al_map_rgb(89, 87, 85);
	static ALLEGRO_COLOR purple = al_map_rgb(178, 102, 255);
	static ALLEGRO_COLOR green	= al_map_rgb(43, 178, 18);
	static ALLEGRO_COLOR red	= al_map_rgb(178, 26, 25);
	static ALLEGRO_COLOR blue	= al_map_rgb(25, 165, 255);
	static ALLEGRO_COLOR darkg	= al_map_rgb(32, 32, 32);

	// visualise nodemap
	for (int x = 0; x < _nodeMap->getWidth(); x++) {
		for (int y = 0; y < _nodeMap->getHeight(); y++) {
			ALLEGRO_COLOR color = gray;

			// if start node..
			if (_start->x == x && _start->y == y) {
				color = green;
			} // else, if the end node..
			else if (_end->x == x && _end->y == y) {
				color = red;
			} // else, if the node isn't traversable
			else if (!_nodeMap->getNode(x, y)->traversable) {
				color = darkg;
			} // else, if it's a path node (in path sequence)
			else if (node_position_in_list(x, y, _searchResult.path)) {
				color = blue;
			} // otherwise, if node was considered
			else if (node_position_in_list(x, y, _searchResult.traversed)) {
				color = purple;
			}

			// x & y position
			float xp = (x * size) + x;
			float yp = (y * size) + y;

			// draw a rectangle in this node position
			al_draw_filled_rectangle(xp, yp, xp + size, yp + size, color);
		}
	}
}
