#pragma once

#include "Entity.h"
#include "Enemy.h"
#include "Wall.h"
#include "Hitbox.h"
#include "Player.h"
#include "LevelManager.h"
#include <memory>

class WorldManager {
public:
	explicit WorldManager();
	void destroy();
	void renderWorld();
	void updateWorld();
	void startGame();

private:
	//box2d World
	b2WorldDef worldDef;
	b2WorldId worldId;

	//Level related param
	std::unique_ptr<LevelManager> levelManager;

	//SFML window
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<sf::View> camera;

};