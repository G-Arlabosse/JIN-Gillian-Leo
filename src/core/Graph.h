#pragma once

#include "box2d/types.h"
#include "SFML/Graphics.hpp"
#include <map>
#include <vector>
#include <queue>
#include "Constants.h"
#include <set>
#include <limits>

const struct node {
	int id;
	b2Vec2 coords;
	int weight, heuristic;
	std::vector<int> transitions;
};

class Graph {
private:
	int width, height;
	std::map<int, std::unique_ptr<struct node>> nodes;

public:
	Graph();
	const void addNode(int x, int y);
	const int getCoordsId(b2Vec2 coords);
	std::vector<b2Vec2> getPath(b2Vec2& start, b2Vec2& goal);
	std::vector<b2Vec2> reconstructPath(const std::map<int, int>& cameFrom, int current_id);
	const void initGraphTransitions();
	const void dumpGraph();
	const void setWidth(int new_width);
	const void setHeight(int new_height);
	sf::VertexArray getPathRender(std::vector<b2Vec2>& path);
};