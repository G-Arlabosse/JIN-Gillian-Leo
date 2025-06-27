#pragma once

#include <map>
#include <vector>
#include "box2d/types.h"
#include <memory>
#include <tmxlite/Map.hpp>
#include "SFMLOrthogonalLayer.hpp"

#include "Enemy.h"	//MUST CHANGE ?
#include "Carrot.h"
#include "Corn.h"
#include "Player.h" //MUST CHANGE ?
#include "Wall.h"
#include <queue>
#include <stack>
#include "Graph.h"
#include "LevelMediator.h"
#include "LevelTransition.h"
#include "WorldTransition.h"
#include "WorldNotifier.h"

const float timeStep = 1 / 60.0f;
const int subStepCount = 8;

const struct callLoadLevel {
	bool isCalled = false;
	bool goBackToLobby = false;
	enum direction dir = direction::NONE;
};

//
class LevelManager: public LevelMediator {
public:
	//Constructor
	explicit LevelManager(WorldNotifier* wn, sf::RenderWindow* window, TextureManager* textureManager);
	explicit LevelManager(TextureManager* textureManager);

	void notifyDamage(int32_t hurtboxId, int damage) override;
	void notifyDeath(int32_t hurtboxId) override;

	// Turns a file to a playable map
        void fileToMap(b2WorldId& worldId, const std::string& name,
                       bool cleared);

	void unloadLevel();
	// Loads the level with the given name inside worldId
  void loadLevel(b2WorldId& worldId, const std::string& level_name, direction dir, bool cleared);

  // Loads the first level inside worldId
  void loadFirstLevel(b2WorldId& worldId);

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
  Wall* createWall(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox);

  // Adds a level transition in the level, usually called by loadLevel
  LevelTransition* createRoomTransition(b2WorldId& worldId, float pos_x, float pos_y, direction dir);

	WorldTransition* createFloorTransition(b2WorldId& worldId, float pos_x, float pos_y, direction dir);

	//Adds the player in the level, usually called by loadLevel
	Player* createPlayer(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox);

	//Adds an enemy in the level, usually called by loadLevel
	Enemy* createEnemy(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox);

	//GETTER: returns the player's position
	b2Vec2 getPlayerPosition();

	void setTempo(int bpm);
	void initTempo();
	
private:
	std::unique_ptr<Graph> levelGraph;
	sf::VertexArray path_render;

	tmx::Map map;
  std::unique_ptr<MapLayer> layer;
	std::map<int32_t,std::unique_ptr<Enemy>> enemies;
	std::unique_ptr<Player> player;
  std::vector<std::unique_ptr<Wall>> walls;
  std::vector<std::unique_ptr<LevelTransition>> level_transitions;
	std::vector<std::unique_ptr<WorldTransition>> world_transitions;

	WorldNotifier* world_notifier;
	struct callLoadLevel callLoadLevel;

	//Texture related param
	TextureManager* textureManager;
	sf::RenderWindow* window;
	sf::Texture texture;

	//Tempo related param
	sf::CircleShape beatIndicator;
	bool inTempo;
	float tempoTimeEntities;
	float tempoTimePlayer;

  float tempoMS = 500;
	float delta = 150;
	float delta2 = 50;
};

