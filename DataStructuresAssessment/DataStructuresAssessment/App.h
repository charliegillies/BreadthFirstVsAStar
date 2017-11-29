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

	NodeSearchResult _aStarSearchResult;
	NodeSearchResult _breadthSearchResult;

	enum Search {
		AStar, Breadth
	};

	Search _search;

	/** Randomize map weights and traversal states. */
	void randomize_map();

	/** Randomize the start and end position of the search. */
	void randomize_start_end();

public:
	App();

	/** Program entry point. */
	void initialize();

	/** Frame logic. */
	void update(float dt);

	/** Frame rendering. */
	void render();

	/** On key up. */
	void on_key_up(int key);

};
