#pragma once

#include <map>
#include <vector>
#include "box2d/types.h"
#include <memory>

#include "Enemy.h"	//MUST CHANGE ?
#include "Player.h" //MUST CHANGE ?
#include "Wall.h"
#include <queue>
#include <stack>
#include "Graph.h"
#include "LevelMediator.h"



const int tempoMS = 500;
const int delta = 100;
const int delta2 = 50;
const float timeStep = 1 / 60.0f;
const int subStepCount = 8;

//
class LevelManager: public LevelMediator {
public:
	//Constructor
	explicit LevelManager(sf::RenderWindow* window);

	void notifyDamage(int32_t hurtboxId, int damage, b2BodyId& hitboxId) override;
	void notifyDeath(int32_t hurtboxId) override;

	//Loads the level with the given id inside worldId
	void loadLevel(b2WorldId& worldId, int levelId);

	/*
	Updates the level
	Calculates if the beat hits
	*/
	void updateLevel(b2WorldId &worldId);

	//Updates all elements in the level each frame
	void updateAll(long clock);

	//Updates all elements when the beat hits
	void updateTempo(long clock);

	//Updates the player, should be moved in player.cpp ?
	//void updatePlayer(b2WorldId& worldId);

	//GETTER: returns true if in the beat hit window
	bool isInTempo();

	//Renders all elements of the level
	void renderEntities(sf::RenderWindow* window);

	//Adds a wall in the level, usually called by loadLevel
	void createWall(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox);

	//Adds the player in the level, usually called by loadLevel
	void createPlayer(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox);

	//Adds an enemy in the level, usually called by loadLevel
	void createEnemy(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox);

	//GETTER: returns the player's position
	b2Vec2 getPlayerPosition();
	
private:
	std::unique_ptr<Graph> levelGraph;
	sf::VertexArray path_render;

	std::map<int32_t,std::unique_ptr<Enemy>> enemies;
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

