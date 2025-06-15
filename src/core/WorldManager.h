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

class WorldManager : public WorldNotifier {
public:
	explicit WorldManager();
	void destroy();
	void renderWorld();
	void updateWorld();
	void startGame();
  std::vector<std::vector<std::string>> makeMap();
  void changeLevel(direction dir);

	void notifyTransition(direction dir) override;

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
  std::vector<std::vector<std::string>> map;
  std::vector<std::vector<bool>> clear_state_map;
  int level_x;
  int level_y;

	std::unique_ptr<TextureManager> textureManager;
	std::unique_ptr<MusicManager> musicManager;

};