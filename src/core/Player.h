#pragma once

#include "Entity.h"

class Player : public Entity {
public:
	Player(const b2WorldId& worldId, float pos_x, float pos_y,
		sf::Texture* texture, LevelMediator *levelMediator, bool renderDebugBoxes);
	void move(b2Vec2& target);
	void attack(b2Vec2 direction, float damage);
	void updateTempo() override;
	void update(long clock);
	void updateInput();
private:
	bool lockAction();
	void unlockAction();

	bool actionLocked;
	long actionLockClock;
};
