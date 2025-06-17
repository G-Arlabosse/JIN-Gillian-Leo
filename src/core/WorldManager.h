#pragma once

#include "Entity.h"
#include "Enemy.h"
#include "Wall.h"
#include "Hitbox.h"
#include "Player.h"
#include "LevelManager.h"
#include "WorldNotifier.h"
#include "TextureManager.h"
#include "MusicManager.h"
#include <memory>

const int nb_floor = 2;

const struct room {
	std::string name;
	bool cleared = false;
};

const struct floor {
	int first_room_x;
	int first_room_y;
	std::vector<std::vector<struct room>> rooms;
	enum musicName music;
};

const struct worldMap {
	std::vector<struct floor> floors;
};

class WorldManager : public WorldNotifier {
public:
	explicit WorldManager();
	void destroy();
	void renderWorld();
	void updateWorld();
	void startGame();
  void makeMap();
  void changeLevel(direction dir);
	void changeFloor(direction dir);

	void notifyTransition(direction dir) override;
	void loadLobby() override;

private:
	//box2d World
	b2WorldDef worldDef;
	b2WorldId worldId;

	//Level related param
	std::unique_ptr<LevelManager> levelManager;

	//SFML window
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<sf::View> camera;

	//Map made of multiple levels
	std::unique_ptr<struct worldMap> worldMap;
  int level_x;
  int level_y;
	int floor;

  int rooms_cleared;
  std::vector<int> rooms_needed;

	std::unique_ptr<TextureManager> textureManager;
	std::unique_ptr<MusicManager> musicManager;

};