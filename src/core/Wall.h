#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "Hurtbox.h"

class Wall {
public:
	Wall(const b2WorldId& worldId, float pos_x, float pos_y, const b2Vec2& hitboxSize, bool renderDebugBoxes);
	void renderWall(sf::RenderWindow* window) const;

private:
	Hurtbox hurtbox;
	bool renderDebugBoxes;
};