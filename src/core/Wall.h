#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "Hitbox.h"

class Wall {
public:
	Wall(const b2WorldId& worldId, float pos_x, float pos_y, const b2Vec2& hitboxSize, bool renderHitboxes);
	void renderWall(sf::RenderWindow* window) const;

private:
	Hitbox hitbox;
	bool renderHitboxes;
};