#pragma once

#include "Entity.h"
#include "Wall.h"
#include "Hitbox.h"
#include <memory>

class WorldManager {
public:
	explicit WorldManager(sf::RenderWindow *window);
	void createEntity(float pos_x, float pos_y, int hp, float hitboxSizeX, float hitboxSizeY, bool showHitbox); //vitual
	void createWall(float pos_x, float pos_y, float hitboxSizeX, float hitboxSizeY, bool showHitbox);
	void renderEntities();
	void updateWorld();
	void updatePlayer();
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void destroy();
	bool isInTempo();
	void updateTempo(long clock);
	void updateAll(long clock);

private:
	b2WorldDef worldDef;
	b2WorldId worldId;
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Wall>> walls;
	std::vector<std::unique_ptr<Hitbox>> hitboxes;
	sf::RenderWindow *window;
	float timeStep = 1 / 60.0f;
	int subStepCount = 8;
	int bpm = 600;
	int delta = 50;
	int delta2 = 30;
	sf::CircleShape beatIndicator;
	bool inTempo;
	long tempoTimeEntities;
	long tempoTimePlayer;

	sf::Texture texture;
};