#include "Player.h"
#include "Constants.h"

b2Vec2 playerHitbox{ 20, 20 };

Player::Player(const b2WorldId& worldId, float pos_x, float pos_y, sf::Texture* texture, bool renderDebugBoxes) :
	Entity(worldId, pos_x, pos_y, 3, playerHitbox, texture, 
		entityType::PLAYER_HURTBOX, entityType::WALL | entityType::ENEMY_HITBOX | entityType::ENEMY_HURTBOX,
		entityType::PLAYER_HITBOX, entityType::ENEMY_HURTBOX,
		renderDebugBoxes),
	test{ 0 }
{}

void Player::move(float x, float y)
{
	hurtbox.move(x, y);
}

void Player::updateTempo() {
	return;
}
