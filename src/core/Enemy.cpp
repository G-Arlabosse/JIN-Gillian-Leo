#include "Enemy.h"
#include "Constants.h"

const int moveAmplitute = 200;
const b2Vec2 movUp = { 0, -moveAmplitute };
const b2Vec2 movRight = { moveAmplitute, 0 };
const b2Vec2 movDown = { 0, moveAmplitute };
const b2Vec2 movLeft = { -moveAmplitute, 0 };
const b2Vec2 movWait = b2Vec2_zero;

b2Vec2 enemyHitbox{ 20, 20 };

const std::vector<b2Vec2> movementPattern = { movRight, movWait, movLeft, movWait,
movDown, movUp, movUp, movDown
};

Enemy::Enemy(const b2WorldId& worldId, float pos_x, float pos_y, sf::Texture* texture, bool renderDebugBoxes) :
	Entity(worldId, pos_x, pos_y, 3, enemyHitbox, texture, 
		entityType::ENEMY_HURTBOX, entityType::WALL | entityType::PLAYER_HITBOX | entityType::PLAYER_HURTBOX, 
		entityType::ENEMY_HITBOX, entityType::PLAYER_HURTBOX,
		renderDebugBoxes),
	patternState{ 0 }
{}

void Enemy::move(float x, float y)
{
	hurtbox.move(x, y);
}

void Enemy::updateTempo() {
	patternState = (patternState + 1) % movementPattern.size();
	hurtbox.move(movementPattern[patternState]);
}
