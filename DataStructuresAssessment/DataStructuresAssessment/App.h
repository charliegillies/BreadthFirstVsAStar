#pragma once

#include "Pathing.h"

/*
 * Centralised application point for 
 */
class App {
private:
	Nodemap* _nodeMap;

public:
	App();

	/** Program entry point. */
	void initialize();

	/** Frame logic. */
	void update(float dt);

	/** Frame rendering. */
	void render();
};
