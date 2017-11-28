#include "App.h"

App::App()
{
	_nodeMap = new Nodemap(10, 10);
}

void App::initialize()
{
	auto n_start = _nodeMap->getNode(0, 0);
	auto n_end = _nodeMap->getNode(9, 9);

	NodeSearchResult result = pathfind(BreadthFirstSearch(), *_nodeMap, n_start, n_end);
}

void App::update(float dt)
{

}

void App::render()
{
	// visualise nodemap
	for (int x = 0; x < _nodeMap->getWidth(); x++) {
		for (int y = 0; y < _nodeMap->getHeight(); y++) {
			// draw node..
			// GRAY - UNUSED
			// GREEN - START
			// RED - END
			// BLUE - TRAVELLED
			// ORANGE - UNTRAVELABLE



		}
	}
}
