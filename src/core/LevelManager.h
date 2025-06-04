#pragma once

#include <map>
#include <vector>
#include "box2d/types.h"
#include <memory>
#include "Enemy.h"
#include "Player.h"
#include "Wall.h"
#include <queue>
#include <stack>
#include "Graph.h"



const int tempoMS = 600;
const int delta = 50;
const int delta2 = 30;
const float timeStep = 1 / 60.0f;
const int subStepCount = 8;

//
class LevelManager {
public:
	explicit LevelManager();

	//Graph related
	std::vector<b2Vec2> reconstructPath(const std::map<int, int>& cameFrom, int current_id);
	std::vector<b2Vec2> getPath(b2Vec2 &start, b2Vec2 &goal);
	void loadLevel(b2WorldId& worldId, int levelId);

	//Updates
	void updateLevel(b2WorldId &worldId);

	void updateAll(long clock);
	void updateTempo(long clock);
	void updatePlayer(b2WorldId& worldId);

	bool isInTempo();

	//Render
	void renderEntities(sf::RenderWindow* window);

	//Creation
	void createWall(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox);
	void createPlayer(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox);
	void createEnemy(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox);

	b2Vec2 getPlayerPosition();
	
private:
	std::unique_ptr<Graph> levelGraph;
	//std::map<int, struct level> levels;
	void renderPath(std::vector<b2Vec2> path);
	sf::VertexArray path_render;

	std::vector<std::unique_ptr<Enemy>> enemies;
	std::unique_ptr<Player> player;
	std::vector<std::unique_ptr<Wall>> walls;

	//Texture related param
	sf::RenderWindow* window;
	sf::Texture texture;

	//Tempo related param
	sf::CircleShape beatIndicator;
	bool inTempo;
	long tempoTimeEntities;
	long tempoTimePlayer;
};

