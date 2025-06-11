#pragma once

#include "box2d/types.h"
#include "SFML/Graphics.hpp"
#include <map>
#include <vector>
#include <queue>
#include "Constants.h"
#include <set>
#include <limits>

const struct transition {
	int neighbor_id;
	int weight;
};
const struct node {
	int id;
	b2Vec2 coords;
	std::vector<struct transition> transitions;
};

class Graph {
private:
	int width, height;
	std::map<int, std::unique_ptr<struct node>> nodes;

	//Reconstructs the path made with A*
	std::vector<b2Vec2> reconstructPath(const std::map<int, int>& cameFrom, int current_id);
public:
	//Constructor
	Graph();

	//When a graph has all its nodes, sets up all the transitions
	const void initGraphTransitions();

	//Add a node to the Graph
	const void addNode(int x, int y);

	//GETTER: returns the id of the node with the given coordinates
	const int getCoordsId(b2Vec2 coords);

	//GETTER: Returns the path found with A*
	std::vector<b2Vec2> getPath(b2Vec2& start, b2Vec2& goal);
	
	//GETTER: Returns a the drawable version of the given path
	sf::VertexArray getPathRender(std::vector<b2Vec2>& path);

	//Dumps the graph
	const void dumpGraph();

	//Inits the graph's width
	const void setWidth(int new_width);

	//Inits the graph's height
  const void setHeight(int new_height);

  // Returns the graph's width
  const int getWidth();

  // Returns the graph's height
  const int getHeight();
};