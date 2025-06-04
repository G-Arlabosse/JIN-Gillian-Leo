#pragma once

#include "SFML/Graphics.hpp"
#include "Hurtbox.h"
#include "Hitbox.h"
#include "TextureHandler.h"
#include <vector>
#include <memory>
#include "Constants.h"

class LevelManager;

class Entity {
public:
	Entity(const b2WorldId &worldId, float pos_x, float pos_y, int hp, b2Vec2& hitboxSize, sf::Texture *texture, 
		uint64_t categoryBitsHurtbox, uint64_t maskBitsHurtbox,
		uint64_t categoryBitsHitbox, uint64_t maskBitsHitbox,
		bool renderDebugBoxes);
	virtual ~Entity() = default;

	void renderEntity(sf::RenderWindow *window);
	void attack(b2WorldId &worldId, b2Vec2 direction, float damage);
	void update(long clock);
	b2Vec2 getPosition();

	virtual void move(float x, float y) = 0;
	virtual void updateTempo() = 0;

protected:
	Hurtbox hurtbox;
	Hitbox hitbox;
	float pos_x;
	float pos_y;
	int maxHp;
	int hp;
	bool renderDebugBoxes;
    TextureHandler texture_handler;


	int patternState;
};