#pragma once

#include "Pathing.h"

/*
 * Centralised application point for 
 */
class App {
private:
	Nodemap* _nodeMap;
	Node* _startNode;
	Node* _endNode;
	NodeSearchResult _searchResult;

public:
	App();

	/** Program entry point. */
	void initialize();

	/** Frame logic. */
	void update(float dt);

	/** Frame rendering. */
	void render();

	/** Randomize map data. */
	void randomize_map();
};
