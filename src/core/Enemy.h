#pragma once

#include "Entity.h"

class Enemy : public Entity {
public:
	Enemy(const b2WorldId& worldId, float pos_x, float pos_y, sf::Texture* texture, bool renderDebugBoxes);
	void move(float x, float y) override;
	void updateTempo() override;
private:
	int patternState;
};