#pragma once

#include "Entity.h"
#include <memory>

class WorldManager {
public:
	explicit WorldManager(sf::RenderWindow *window);
	void createEntity(float pos_x, float pos_y, int hp, float hitboxSizeX, float hitboxSizeY, bool showHitbox); //vitual
	void renderEntities();
	void updateWorld();
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void destroy();

private:
	b2WorldDef worldDef;
	b2WorldId worldId;
	std::vector<std::unique_ptr<Entity>> entities;
	sf::RenderWindow *window;
	float timeStep = 1 / 60.0f;
	int subStepCount = 8;
};