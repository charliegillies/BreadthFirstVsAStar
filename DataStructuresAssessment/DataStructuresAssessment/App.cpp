#include "App.h"

#include <string>

#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

App::App() { 
	_search = Search::AStar;
	srand((unsigned)time(NULL));
}

const int WIDTH = 30;
const int HEIGHT = 18;

void App::initialize()
{
	// build a nodemap
	_nodeMap = new Nodemap(WIDTH, HEIGHT);

	randomize_start_end();
	randomize_map();

	_aStarSearchResult = pathfind(AStarSearch(), *_nodeMap, _startNode, _endNode);
	_breadthSearchResult = pathfind(BreadthFirstSearch(), *_nodeMap, _startNode, _endNode);
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
	// size of a node
	static const int size = 32;

	// default colours
	static ALLEGRO_COLOR gray	= al_map_rgb(89, 87, 85);
	static ALLEGRO_COLOR purple = al_map_rgb(178, 102, 255);
	static ALLEGRO_COLOR green	= al_map_rgb(43, 178, 18);
	static ALLEGRO_COLOR red	= al_map_rgb(178, 26, 25);
	static ALLEGRO_COLOR blue	= al_map_rgb(25, 165, 255);
	static ALLEGRO_COLOR darkg	= al_map_rgb(32, 32, 32);
	static ALLEGRO_COLOR white	= al_map_rgb(255, 255, 255);

	// default font
	static ALLEGRO_FONT* largeArial	= al_load_ttf_font("arial.ttf", 36, 0);
	static ALLEGRO_FONT* smallArial = al_load_ttf_font("arial.ttf", 18, 0);

	// current result target
	NodeSearchResult result = (_search == AStar) ? _aStarSearchResult : _breadthSearchResult;

	// visualise nodemap
	for (int x = 0; x < _nodeMap->getWidth(); x++) {
		for (int y = 0; y < _nodeMap->getHeight(); y++) {
			ALLEGRO_COLOR color = gray;

			Node* node = _nodeMap->getNode(x, y);

			// if start node..
			if (_startNode->x == x && _startNode->y == y) {
				color = green;
			} // else, if the end node..
			else if (_endNode->x == x && _endNode->y == y) {
				color = red;
			} // else, if the node isn't traversable
			else if (!node->traversable) {
				color = darkg;
			} // else, if it's a path node (in path sequence)
			else if (node_position_in_list(x, y, result.path)) {
				color = blue;
			} // otherwise, if node was considered
			else if (node_position_in_list(x, y, result.traversed)) {
				color = purple;
			}

			// x & y position
			float xp = (x * size) + x;
			float yp = (y * size) + y;

			// draw a rectangle in this node position
			al_draw_filled_rectangle(xp, yp, xp + size, yp + size, color);

			if (node->traversable) {
				// draw the weight of the node 
				std::string nodeweight = std::to_string(_nodeMap->getNode(x, y)->weight);
				al_draw_text(smallArial, white, xp + 10, yp + 6, 0, nodeweight.data());
			}
		}
	}

	// draw search time on screen
	std::string searchtime = std::to_string(result.time) + "s";
	// indicate if search is A* or BFS
	searchtime += (_search == AStar) ? " (A*)" : " (BF)";
	// draw the search time 
	al_draw_text(largeArial, white, 120, 650, ALLEGRO_ALIGN_CENTER, searchtime.data());
}

void App::on_key_up(int key)
{
	if (key == ALLEGRO_KEY_SPACE) {
		// switch from A* to BFS
		_search = (_search == AStar) ? Breadth : AStar;
	}
	else if (key == ALLEGRO_KEY_R) {
		// Refresh the searches
		_aStarSearchResult = pathfind(AStarSearch(), *_nodeMap, _startNode, _endNode);
		_breadthSearchResult = pathfind(BreadthFirstSearch(), *_nodeMap, _startNode, _endNode);
	}
	else if (key == ALLEGRO_KEY_F) {
		// delete old map, build a new nodemap
		delete _nodeMap;
		initialize();
	}
}

void App::randomize_start_end()
{
	// w & h
	int w = _nodeMap->getWidth();
	int h = _nodeMap->getHeight();

	// random start x & y position
	int sx = rand() % w;
	int sy = rand() % h;

	// random end x & y position
	int ex = rand() % w;
	int ey = rand() % h;

	// ensure that start and end aren't the same
	while (sx == ex)
		ex = rand() % w;
	while (sy == ey)
		ey = rand() % h;

	// set the start & end nodes
	_startNode = _nodeMap->getNode(sx, sy);
	_endNode = _nodeMap->getNode(ex, ey);
}

void App::randomize_map()
{
	int w = _nodeMap->getWidth();
	int h = _nodeMap->getHeight();
	int r = (w * h) / 8;

	// set a set of random nodes to be untraversable..
	for (int i = 0; i < r; i++) {
		// random x & y coords
		int ix = rand() % w;
		int iy = rand() % h;

		// skip if start or end node
		if (ix == _startNode->x && iy == _startNode->y)
			continue;
		if (ix == _endNode->x && iy == _endNode->y)
			continue;

		_nodeMap->getNode(ix, iy)->traversable = false;
	}

	// change the weights of some random nodes..
	for (int i = 0; i < r; i++) {
		// random x & y coords
		int ix = rand() % w;
		int iy = rand() % h;

		// skip if start or end node
		if (ix == _startNode->x && iy == _startNode->y)
			continue;
		if (ix == _endNode->x && iy == _endNode->y)
			continue;

		// set a random weight between 1 and 9
		_nodeMap->getNode(ix, iy)->weight = rand() % 9 + 1;
	}
}
