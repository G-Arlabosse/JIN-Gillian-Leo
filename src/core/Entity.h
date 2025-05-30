#pragma once

#include "SFML/Graphics.hpp"
#include "Hitbox.h"

class Entity {
public:
	Entity(const b2WorldId &worldId, float pos_x, float pos_y, int hp, b2Vec2& hitboxSize, bool renderHitboxes);
	void renderEntity(sf::RenderWindow *window);
	void move(float x, float y);

private:
	Hitbox hitbox;
	float pos_x;
	float pos_y;
	int maxHp;
	int hp;
	bool renderHitboxes;
};