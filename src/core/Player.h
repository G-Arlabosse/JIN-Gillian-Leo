#pragma once

#include "Entity.h"

class Player : public Entity {
public:
	Player(const b2WorldId& worldId, float pos_x, float pos_y,
        TextureManager* textureManager, float tempo,
        LevelMediator* levelMediator, bool renderDebugBoxes);
	void move(b2Vec2& target);
	void attack(b2Vec2 direction, float damage);
	void update(long clock, const sf::RenderWindow* window, bool inPlayerTempoWindow);
	bool updateInput(const sf::RenderWindow *window);
	void notifyEndTempo();
private:
	bool lockAction();
	void unlockAction();

	bool actionLocked;
	long actionLockClock;
	int tempoStreak;
	bool hasMovedTempo;
};
