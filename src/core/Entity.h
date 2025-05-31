#pragma once

#include "SFML/Graphics.hpp"
#include "Hurtbox.h"
#include "Hitbox.h"
#include <vector>
#include <memory>

class Entity {
public:
	Entity(const b2WorldId &worldId, float pos_x, float pos_y, int hp, b2Vec2& hitboxSize, sf::Texture *texture, bool renderDebugBoxes);
	void renderEntity(sf::RenderWindow *window);
	void move(float x, float y);
	void updateTempo();
	void attack(b2WorldId &worldId, b2Vec2 direction, float damage);
	void update(long clock);

private:
	Hurtbox hurtbox;
	Hitbox hitbox;
	float pos_x;
	float pos_y;
	int maxHp;
	int hp;
	bool renderDebugBoxes;
	sf::Sprite sprite;

	int patternState;
};